#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>
#include <QLabel>
#include <QSettings>
#include <QMetaProperty>
#include "canvas.h"
#include "tools.h"
#include "history.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->actionGrid_draw->setChecked(settings.value("grid_draw").toBool());
    ui->actionGrid_snap->setChecked(settings.value("grid_snap").toBool());

    m_scene = new Scene(this);

    m_scene->setZoom(settings.value("zoom",QVariant(1.0)).toDouble());

    m_history = new History(m_scene,this);

    m_tools = new Tools(m_history,this);

    QScrollArea* scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_canvas = new Canvas(m_tools,m_scene,0);

    scroll->setWidget(m_canvas);

    setCentralWidget(scroll);

    m_zoom_status = new QLabel();
    statusBar()->addPermanentWidget(m_zoom_status);

    connect(ui->actionZoom_In,SIGNAL(triggered()),m_scene,SLOT(incrementZoom()));
    connect(ui->actionZoom_Out,SIGNAL(triggered()),m_scene,SLOT(decrementZoom()));
    connect(ui->actionZoom_Actual,SIGNAL(triggered()),m_scene,SLOT(actualZoom()));

    connect(m_scene,SIGNAL(zoomChanged()),this,SLOT(onCanvasZoomChanged()));
    onCanvasZoomChanged();

    m_canvas->setDrawGrid(ui->actionGrid_draw->isChecked());
    connect(ui->actionGrid_draw,SIGNAL(toggled(bool)),m_canvas,SLOT(setDrawGrid(bool)));

    m_canvas->setSnapToGrid(ui->actionGrid_snap->isChecked());
    connect(ui->actionGrid_snap,SIGNAL(toggled(bool)),m_canvas,SLOT(setSnapToGrid(bool)));

    ui->actionUndo->setEnabled(m_history->undoAvaliable());
    ui->actionRedo->setEnabled(m_history->redoAvaliable());

    connect(m_history,SIGNAL(changed()),this,SLOT(onHistoryChanged()));

    connect(ui->actionUndo,SIGNAL(triggered()),m_history,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),m_history,SLOT(redo()));

    connect(m_scene,SIGNAL(changed()),m_canvas,SLOT(repaint()));

    m_tools_actions.push_back(ui->actionTool_edit);
    connect(ui->actionTool_edit,SIGNAL(triggered()),m_tools,SLOT(activateEditTool()));
    m_tools_actions.push_back(ui->actionTool_box);
    connect(ui->actionTool_box,SIGNAL(triggered()),m_tools,SLOT(activateBoxTool()));
    m_tools_actions.push_back(ui->actionTool_circle);
    connect(ui->actionTool_circle,SIGNAL(triggered()),m_tools,SLOT(activateCircleTool()));
    m_tools_actions.push_back(ui->actionTool_polygon);
    connect(ui->actionTool_polygon,SIGNAL(triggered()),m_tools,SLOT(activatePolygonTool()));

    QActionGroup* group = new QActionGroup(this);
    foreach (QAction* a,m_tools_actions) {
        a->setActionGroup(group);
    }

    connect(m_scene,SIGNAL(textChanged()),this,SLOT(onSceneTextChanged()));

    connect(m_tools,SIGNAL(changed()),this,SLOT(onToolChanged()));
    onToolChanged();
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
     QSettings settings;
     settings.setValue("geometry", saveGeometry());
     settings.setValue("grid_draw",ui->actionGrid_draw->isChecked());
     settings.setValue("grid_snap",ui->actionGrid_snap->isChecked());
     settings.setValue("zoom",m_scene->zoom());
     QMainWindow::closeEvent(event);
 }

void MainWindow::saveConfig(QSettings& settings,
                             const QString& name,
                             const QObject* object) {
    settings.beginGroup(name);
    const QMetaObject *metaobject = object->metaObject();
     int count = metaobject->propertyCount();
     for (int i=0; i<count; ++i) {
         QMetaProperty metaproperty = metaobject->property(i);
         const char *name = metaproperty.name();
         QVariant value = object->property(name);
         settings.setValue(name,value);
     }
    settings.endGroup();
}

void MainWindow::readConfig(QSettings& settings,
                             const QString& name,
                             QObject* object) {
    settings.beginGroup(name);
    const QMetaObject *metaobject = object->metaObject();
     int count = metaobject->propertyCount();
     for (int i=0; i<count; ++i) {
         QMetaProperty metaproperty = metaobject->property(i);
         const char *name = metaproperty.name();
         if (settings.contains(name))
             object->setProperty(name,settings.value(name));
      }
    settings.endGroup();
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::onCanvasZoomChanged() {
    ui->actionZoom_In->setEnabled( m_scene->zoomInAllowed() );
    ui->actionZoom_Out->setEnabled( m_scene->zoomOutAllowed() );
    ui->actionZoom_Actual->setEnabled( m_scene->zoomActualAllowed() );

    QString zoomText;
    if ( m_scene->zoom()>1.0 ) {
        zoomText = "%1:1";
        zoomText = zoomText.arg(QString::number(int(m_scene->zoom())));
    } else {
        zoomText = "1:%1";
        zoomText = zoomText.arg(QString::number(int(1.0f/m_scene->zoom())));
    }
    zoomText = QString::number(int(m_scene->zoom()*100)) + QString("% (") + zoomText + QString(")");
    m_zoom_status->setText(zoomText);
}

void MainWindow::onHistoryChanged() {
    ui->actionUndo->setEnabled(m_history->undoAvaliable());
    ui->actionUndo->setText(m_history->undoText());
    ui->actionUndo->setToolTip(m_history->undoText());
    ui->actionRedo->setEnabled(m_history->redoAvaliable());
    ui->actionRedo->setText(m_history->redoText());
    ui->actionRedo->setToolTip(m_history->redoText());
}

void MainWindow::onSceneTextChanged() {
    statusBar()->showMessage(m_scene->text());
}

void MainWindow::onToolChanged() {

    QAction* activeTool = 0;
    switch (m_tools->toolType()) {
        case ToolTypeBox:
            activeTool = ui->actionTool_box;
            break;
        case ToolTypeCircle:
            activeTool = ui->actionTool_circle;
            break;
        case ToolTypePolygon:
            activeTool = ui->actionTool_polygon;
            break;
        case ToolTypeEdit:
            activeTool = ui->actionTool_edit;
            break;
    }
    if (activeTool) {
        activeTool->setChecked(true);
        /*foreach( QAction* a, m_tools_actions ) {
            if (a!=activeTool) {
                a->setChecked(false);
            }
        }*/
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
