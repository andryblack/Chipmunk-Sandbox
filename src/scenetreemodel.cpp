#include "scenetreemodel.h"
#include "scenetreeitem.h"
#include "scene.h"
#include "body.h"
#include "primitive.h"
#include <QIcon>
#include <QFont>
#include <QBrush>

SceneTreeModel::SceneTreeModel(Scene *scene, QObject *parent) :
    QAbstractItemModel(parent),m_root(0),m_scene(scene)
{

}

SceneTreeModel::~SceneTreeModel() {
    delete m_root;
}

int SceneTreeModel::columnCount(const QModelIndex &/*parent*/) const
 {
     return 1;
 }

 QVariant SceneTreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role == Qt::DecorationRole) {
         return QIcon(static_cast<SceneTreeItem*>(index.internalPointer())->iconFile());
     }

     if (role == Qt::FontRole) {
         if ( static_cast<SceneTreeItem*>(index.internalPointer())->active() ) {
             QFont fnt = QFont();
             fnt.setBold(true);
             return fnt;
         }
     }

     if (role == Qt::BackgroundRole) {
         if ( static_cast<SceneTreeItem*>(index.internalPointer())->active() ) {
             return QBrush(QColor(0xFF,0xFE,0xA9));
         }
     }

     if (role != Qt::DisplayRole)
         return QVariant();

     return static_cast<SceneTreeItem*>(index.internalPointer())->name();
 }

 Qt::ItemFlags SceneTreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant SceneTreeModel::headerData(int /*section*/, Qt::Orientation /*orientation*/,
                                 int /*role*/) const
  {
      return QVariant();
  }

  QModelIndex SceneTreeModel::index(int row, int column, const QModelIndex &parent)
              const
  {
      if (!hasIndex(row, column, parent))
          return QModelIndex();

      if (!parent.isValid()) {
          if (row<m_scene->bodysCount())
            return createIndex(row,column,m_scene->body(row));
          return QModelIndex();
      }
      SceneTreeItem* parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());
      if (row<parentItem->childCount()) {
          return createIndex(row,column,parentItem->child(row));
      }
      return QModelIndex();
  }

  QModelIndex SceneTreeModel::parent(const QModelIndex &index) const
  {
      if (!index.isValid())
          return QModelIndex();
      SceneTreeItem* obj = static_cast<SceneTreeItem*>(index.internalPointer());
      if (!obj)
        return QModelIndex();
      SceneTreeItem* parentItem = obj->parent();
      if (parentItem) {
          return createIndex(parentItem->indexOf(obj),0,parentItem);
      }
      return QModelIndex();
  }

  int SceneTreeModel::rowCount(const QModelIndex &parent) const
  {
      if (!parent.isValid())
          return m_scene->bodysCount();

      if (parent.column() > 0)
          return 0;

      SceneTreeItem* obj = static_cast<SceneTreeItem*>(parent.internalPointer());

      return obj ? obj->childCount() : 0;
  }

  void SceneTreeModel::update() {
      emit layoutChanged();
  }
