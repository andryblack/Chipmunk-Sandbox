#include "primitive.h"
#include "canvas.h"
#include "primitivemarker.h"
#include "body.h"

Primitive::Primitive(Body* scene,QObject *parent) :
    SceneTreeItem(parent), m_body(scene)
{
    m_friction = 1.0;
    m_elasticity = 1.0;
    m_sensor = false;
    m_collisionType = 0;
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

SceneTreeItem* Primitive::child(int /*indx*/) {
    return 0;
}

int Primitive::indexOf(SceneTreeItem* /*child*/) {
    return -1;
}

SceneTreeItem* Primitive::parent() const {
    return m_body;
}

void Primitive::select( bool s ) {
    if (m_body) m_body->selectPrimitive(this,s);
}

bool Primitive::selected() const {
    if (m_body) return m_body->primitiveSelected(this);
    return false;
}
