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
#include "sceneselectionmodel.h"
#include "primitive.h"
#include "body.h"
#include "commands/createbodycommand.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    ui->splitter->restoreState(settings.value("splitter").toByteArray());
    ui->splitter_2->restoreState(settings.value("splitter2").toByteArray());

    ui->actionGrid_draw->setChecked(settings.value("grid_draw").toBool());
    ui->actionGrid_snap->setChecked(settings.value("grid_snap").toBool());
    bool ok = false;
    int splitterVal = settings.value("propertiesSplit").toInt(&ok);
    if ( ok )
        ui->propertyBrowser->setSplitterPosition(splitterVal);

    m_history = new History(this);

    m_scene = new Scene(m_history,this);

    m_scene->setZoom(settings.value("zoom",QVariant(1.0)).toDouble());


    m_tools = new Tools(m_scene,this);


    ui->canvasWidget->init(m_tools,m_scene);

    m_zoom_status = new QLabel();
    statusBar()->addPermanentWidget(m_zoom_status);

    connect(ui->actionZoom_In,SIGNAL(triggered()),m_scene,SLOT(incrementZoom()));
    connect(ui->actionZoom_Out,SIGNAL(triggered()),m_scene,SLOT(decrementZoom()));
    connect(ui->actionZoom_Actual,SIGNAL(triggered()),m_scene,SLOT(actualZoom()));

    connect(m_scene,SIGNAL(zoomChanged()),this,SLOT(onCanvasZoomChanged()));
    onCanvasZoomChanged();

    ui->canvasWidget->setDrawGrid(ui->actionGrid_draw->isChecked());
    connect(ui->actionGrid_draw,SIGNAL(toggled(bool)),ui->canvasWidget,SLOT(setDrawGrid(bool)));

    ui->canvasWidget->setSnapToGrid(ui->actionGrid_snap->isChecked());
    connect(ui->actionGrid_snap,SIGNAL(toggled(bool)),ui->canvasWidget,SLOT(setSnapToGrid(bool)));

    ui->actionUndo->setEnabled(m_history->undoAvaliable());
    ui->actionRedo->setEnabled(m_history->redoAvaliable());

    connect(m_history,SIGNAL(changed()),this,SLOT(onHistoryChanged()));

    connect(ui->actionUndo,SIGNAL(triggered()),m_scene,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered()),m_scene,SLOT(redo()));

    connect(m_scene,SIGNAL(changed()),ui->canvasWidget,SLOT(repaint()));

    m_tools_actions.push_back(ui->actionTool_edit);
    connect(ui->actionTool_edit,SIGNAL(triggered()),m_tools,SLOT(activateEditTool()));
    m_tools_actions.push_back(ui->actionTool_box);
    connect(ui->actionTool_box,SIGNAL(triggered()),m_tools,SLOT(activateBoxTool()));
    m_tools_actions.push_back(ui->actionTool_circle);
    connect(ui->actionTool_circle,SIGNAL(triggered()),m_tools,SLOT(activateCircleTool()));
    m_tools_actions.push_back(ui->actionTool_polygon);
    connect(ui->actionTool_polygon,SIGNAL(triggered()),m_tools,SLOT(activatePolygonTool()));
    m_tools_actions.push_back(ui->actionTool_body);
    connect(ui->actionTool_body,SIGNAL(triggered()),m_tools,SLOT(activateBodyTool()));

    QActionGroup* group = new QActionGroup(this);
    foreach (QAction* a,m_tools_actions) {
        a->setActionGroup(group);
    }

    ui->propertyBrowser->setHistory(m_history);

    connect(m_tools,SIGNAL(changed()),this,SLOT(onToolChanged()));
    onToolChanged();

    ui->treeView->setModel(m_scene);
    m_scene_selection = new SceneSelectionModel(m_scene,this);
    ui->treeView->setSelectionModel(m_scene_selection);
    connect(m_scene_selection,SIGNAL(selectByThree(SceneTreeItem*)),this,SLOT(onSelectedByTree(SceneTreeItem*)));

    connect(m_scene,SIGNAL(textChanged()),this,SLOT(onSceneTextChanged()));
    connect(m_scene,SIGNAL(changed()),this,SLOT(onSceneChanged()));
    connect(m_scene,SIGNAL(selectionChanged()),this,SLOT(onSceneSelectionChanged()));
    connect(m_scene_selection,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(onSceneSelectionChanged()));

    QString lastFile = settings.value("lastFile").toString();
    if (lastFile.isEmpty()) {
       m_scene->makeNew(QSize(800,600));
    } else {
       m_scene->load(lastFile);
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
    if (m_history->haveUnsavedChanges()) {
        if (!askSave()) {
           event->ignore();
           return;
        }
    }

     QSettings settings;
     settings.setValue("geometry", saveGeometry());
     settings.setValue("splitter",ui->splitter->saveState());
     settings.setValue("splitter2",ui->splitter_2->saveState() );
     settings.setValue("propertiesSplit",ui->propertyBrowser->splitterPosition() );
     settings.setValue("grid_draw",ui->actionGrid_draw->isChecked());
     settings.setValue("grid_snap",ui->actionGrid_snap->isChecked());
     settings.setValue("zoom",m_scene->zoom());
     settings.setValue("lastFile",m_scene->fileName());
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

void MainWindow::onSelectedByTree(SceneTreeItem *item)
{
    if (qobject_cast<Body*>(item)) {
        m_tools->activateBodyTool();
    } else if (qobject_cast<Primitive*>(item)) {
        m_tools->activateEditTool();
    }
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
    onSceneChanged();
}

void MainWindow::onSceneTextChanged() {
    statusBar()->showMessage(m_scene->text());
}

void MainWindow::onSceneChanged() {
    //m_scene_model->update();
    ui->canvasWidget->onZoomChanged();
    QString doc = m_scene->fileName();
    if (doc.isEmpty()) {
        doc = "untitled";
    }
    if (m_history->haveUnsavedChanges()) {
        doc = "*" + doc;
        ui->actionSave->setEnabled(true);
    } else {
        ui->actionSave->setEnabled(false);
    }
    setWindowTitle(doc);
}

void MainWindow::onSceneSelectionChanged() {
    QList< SceneTreeItem* > items;
    foreach (Primitive* p,  m_scene->selectedPrimitives() ) {
        items.push_back( p );
    }
    if ( items.empty() ) {
        foreach (Body* b,  m_scene->selectedBodies() ) {
            items.push_back( b );
        }
    }
    ui->propertyBrowser->setSelectedObjects( items );
    ui->canvasWidget->repaint();
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
        case ToolTypeBody:
            activeTool = ui->actionTool_body;
            break;
        default:
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


void MainWindow::on_toolButtonAddBody_clicked()
{
    Body* b = new DynamicBody(m_scene,"dynamic",m_scene);
    m_scene->execCommand( new CreateBodyCommand(b,m_scene) );
}

void MainWindow::on_toolButtonAddStaticBody_clicked()
{
    Body* b = new StaticBody(m_scene,"static",m_scene);
    m_scene->execCommand( new CreateBodyCommand(b,m_scene) );
}

bool MainWindow::askSave() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Unsaved");
    msgBox.setText(m_scene->fileName());
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText("File not saved");
    msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
     switch (ret) {
       case QMessageBox::Save:
           saveScene();
           return true;
           break;
       case QMessageBox::Discard:
           return true;
           break;
       default:
           break;
     }
    return false;
}

bool MainWindow::saveIfNeeded() {
    if (m_history->haveUnsavedChanges()) {
        return askSave();
    }
    return true;
}

void MainWindow::saveScene() {
    QString fn = m_scene->fileName();
    if (fn.isEmpty()) {
        on_actionSave_as_triggered();
    } else {
        m_scene->save();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
        QString(), tr("Scene files (*.scene);;All Files (*)"));
    if (!fn.isEmpty() && fn!=m_scene->fileName()) {
        if (saveIfNeeded()) {
            m_scene->load(fn);
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    if(saveIfNeeded()) {
        m_scene->makeNew(m_scene->worldSize());
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                  QString(),
                                              tr("Scene files (*.scene);;All Files (*)"));
    if (fn.isEmpty())
        return;
    m_scene->save(fn);
}

void MainWindow::on_actionSave_triggered()
{
    saveScene();
}
