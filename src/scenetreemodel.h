#ifndef SCENETREEMODEL_H
#define SCENETREEMODEL_H

#include <QAbstractItemModel>

class SceneTreeItem;
class Scene;

class SceneTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SceneTreeModel(Scene* scene,QObject *parent = 0);
    ~SceneTreeModel();
    
    QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

signals:
    
public slots:
    
     void update();

private:

     SceneTreeItem* m_root;

     Scene* m_scene;
};

#endif // SCENETREEMODEL_H
