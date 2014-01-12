#include "sceneselectionmodel.h"
#include "scenetreeitem.h"
#include "scene.h"
#include "primitive.h"
#include "body.h"

SceneSelectionModel::SceneSelectionModel(Scene* scene, QObject *parent) :
    QItemSelectionModel(scene,parent),m_scene(scene)
{
    connect( m_scene,SIGNAL(selectionChanged()),this,SLOT(onSceneSelectionChanged()));
    connect( m_scene,SIGNAL(selectionReset()),this,SLOT(reset()));
}


void	SceneSelectionModel::select ( const QModelIndex & index, QItemSelectionModel::SelectionFlags command ) {
    if (index.isValid()) {

        SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());

        if (command&Select) {
            emit selectByThree(item);

            item->select(true);
        } else if (command&Deselect) {
            item->select(false);
        }
    }
}


void SceneSelectionModel::select(const QItemSelection &selection, QItemSelectionModel::SelectionFlags command) {

    QModelIndexList indexes = selection.indexes();
    if (command&Clear) {
        m_scene->clearSelection();
    }
    foreach ( const QModelIndex & index, indexes ) {
        if (index.isValid()) {
            SceneTreeItem* item = static_cast<SceneTreeItem*>(index.internalPointer());
            if (command&Select) {
                emit selectByThree(item);

                item->select(true);
            } else if (command&Deselect) {
                item->select(false);
            }
        }
    }
    QItemSelectionModel::select(selection,command);
}

void SceneSelectionModel::onSceneSelectionChanged() {
    Primitive* p = m_scene->selected();
    QModelIndex indx;
    if (p) {
        QModelIndex pi = model()->index(m_scene->bodyIndex(p->body()),0);
        indx = model()->index(p->body()->primitiveIndex(p),0,pi);
    }
    QItemSelection new_selection;
    new_selection.push_back(QItemSelectionRange(indx,indx));
    QItemSelectionModel::select(new_selection,ClearAndSelect);
}
