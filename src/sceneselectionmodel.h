#ifndef SCENESELECTIONMODEL_H
#define SCENESELECTIONMODEL_H

#include <QItemSelectionModel>

class Scene;

class SceneSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    explicit SceneSelectionModel( Scene* scene, QObject *parent = 0);
    
signals:
    void selectByThree();
public slots:
    virtual void select( const QModelIndex & index, QItemSelectionModel::SelectionFlags command );
    virtual void select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command);

private slots:
    void onSceneSelectionChanged();
private:
    Scene*  m_scene;
};

#endif // SCENESELECTIONMODEL_H
