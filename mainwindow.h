#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
class MainWindow;
}

class Canvas;
class QLabel;
class Tools;
class History;
class Scene;
class QSettings;

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
    void onSceneTextChanged();
private:
    Ui::MainWindow *ui;
    Tools*  m_tools;
    Canvas* m_canvas;
    QLabel* m_zoom_status;
    Scene*  m_scene;
    History*    m_history;
    QVector<QAction*> m_tools_actions;
};

#endif // MAINWINDOW_H
