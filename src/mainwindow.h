#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class QLabel;
class Tools;
class History;
class Scene;
class QSettings;
class SceneTreeModel;
class SceneSelectionModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void saveConfig(QSettings& config,const QString& name,const QObject* obj);
    void readConfig(QSettings& config,const QString& name,QObject* obj);
protected slots:
    void onCanvasZoomChanged();
    void onHistoryChanged();
    void onToolChanged();
    void onSceneChanged();
    void onSceneTextChanged();
    void onSceneSelectionChanged();
private slots:
    void on_toolButtonAddBody_clicked();

    void on_toolButtonAddStaticBody_clicked();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    Tools*  m_tools;
    QLabel* m_zoom_status;
    Scene*  m_scene;
    History*    m_history;
    QVector<QAction*> m_tools_actions;
    SceneSelectionModel*    m_scene_selection;
    bool saveIfNeeded();
    void saveScene();
    bool askSave();
};

#endif // MAINWINDOW_H
