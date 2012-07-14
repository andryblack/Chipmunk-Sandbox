#include "sceneselectionmodel.h"
#include "scenetreeitem.h"
#include "scene.h"

SceneSelectionModel::SceneSelectionModel(Scene* scene, QAbstractItemModel *model, QObject *parent) :
    QItemSelectionModel(model,parent),m_scene(scene)
{
    connect( m_scene,SIGNAL(selectionChanged()),this,SLOT(onSceneSelectionChanged()));
}


void	SceneSelectionModel::select ( const QModelIndex & index, QItemSelectionModel::SelectionFlags command ) {
    emit selectByThree();

    SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
    if (command&Select) {
        item->select(true);
    } else if (command&Deselect) {
        item->select(false);
    }
}

void SceneSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command) {
    emit selectByThree();

    QModelIndexList indexes = selection.indexes();
    if (command&Clear) {
        m_scene->clearSelection();
    }
    foreach ( const QModelIndex & index, indexes ) {
        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
        if (command&Select) {
            item->select(true);
        } else if (command&Deselect) {
            item->select(false);
        }
    }
    QItemSelectionModel::select(selection,command);
}

void SceneSelectionModel::onSceneSelectionChanged() {
    Primitive* p = m_scene->selected();
    QModelIndex indx;
    if (p) {

    }
    QItemSelection new_selection;
    new_selection.push_back(QItemSelectionRange(indx,indx));
    QItemSelectionModel::select(new_selection,ClearAndSelect);
}
