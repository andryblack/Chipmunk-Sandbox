#include "primitive.h"
#include "canvas.h"
#include "primitivemarker.h"
#include "body.h"

Primitive::Primitive(Body* scene,QObject *parent) :
    SceneTreeItem(parent), m_body(scene)
{
}

Body*  Primitive::body() {
    return m_body;
}

qreal   Primitive::sceneZoom() const {
    return m_body ? m_body->sceneZoom() : 1.0;
}

int Primitive::childCount() const {
    return 0;
}

SceneTreeItem* Primitive::child(int indx) {
    return 0;
}

int Primitive::indexOf(SceneTreeItem* /*child*/) {
    return -1;
}

SceneTreeItem* Primitive::parent() const {
    return m_body;
}

