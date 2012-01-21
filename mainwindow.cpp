#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollArea>
#include <QLabel>
#include "canvas.h"
#include "tools.h"
#include "history.h"
#include "scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new Scene(this);

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

    ui->actionGrid_draw->setChecked(m_canvas->drawGrid());
    connect(ui->actionGrid_draw,SIGNAL(toggled(bool)),m_canvas,SLOT(setDrawGrid(bool)));

    ui->actionGrid_snap->setChecked(m_canvas->snapToGrid());
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

    QActionGroup* group = new QActionGroup(this);
    foreach (QAction* a,m_tools_actions) {
        a->setActionGroup(group);
    }

    connect(m_scene,SIGNAL(textChanged()),this,SLOT(onSceneTextChanged()));

    connect(m_tools,SIGNAL(changed()),this,SLOT(onToolChanged()));
    onToolChanged();
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
    ui->actionRedo->setEnabled(m_history->redoAvaliable());
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
