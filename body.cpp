#include "body.h"
#include "scene.h"
#include "primitive.h"
#include <QPainter>

Body::Body(Scene* scene,const QString& name,QObject *parent) :
    SceneTreeItem(parent),m_scene(scene)
{
    setObjectName(name);
}


int Body::childCount() const {
    return primitivesCount();
}

SceneTreeItem* Body::child(int indx) {
    return primitive(indx);
}
int Body::indexOf(SceneTreeItem* child) {
    return m_primitives.indexOf(static_cast<Primitive*>(child));
}

SceneTreeItem* Body::parent() const {
    return 0;
}

qreal Body::sceneZoom() const {
    return m_scene ? m_scene->zoom() : 1.0;
}

void Body::execCommand( Command* cmd ) {
    if (m_scene) {
        m_scene->execCommand( cmd );
    }
}

int Body::primitivesCount() const {
    return m_primitives.size();
}

Primitive* Body::primitive(int indx) {
    return m_primitives.at(indx);
}

int Body::primitiveIndex( Primitive* p) const {
    return m_primitives.indexOf(p);
}

void Body::addPrimitive(Primitive* p) {
    p->setParent(this);
    m_primitives.push_back(p);
    emit changed();
}

void Body::removePrimitive(Primitive* p) {
    m_primitives.removeAll(p);
    emit changed();
}

Primitive* Body::getPrimitiveAtPoint(const QPointF &pnt) {
    foreach( Primitive* p, m_primitives) {
        if ( p->isPointInside(pnt) )
            return p;
    }
    return 0;
}

bool Body::active() const {
    return m_scene->activeBody()==this;
}

void Body::select( bool s) {
    if (s) {
        m_scene->addSelected(this);
    } else {
        m_scene->removeSelected(this);
    }
}

void Body::selectPrimitive( Primitive* p , bool select) {
    if (select) {
        m_scene->addSelected(p);
    } else {
        m_scene->removeSelected(p);
    }
}

void Body::Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const {
    qreal opc = painter->opacity();
    if (!active())
        painter->setOpacity(opc * 0.3);
    foreach ( Primitive* p, m_primitives ) {
        if (selected.indexOf(p)==-1) {
            p->Draw( canvas, painter );
        }
    }
    painter->setOpacity(opc);
}

void Body::Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const {
    selected->Draw(canvas,painter);
}

StaticBody::StaticBody(Scene* scene,const QString &name, QObject *parent) :
    Body(scene,name,parent) {

}



QString StaticBody::iconFile() const {
    if (active())
        return ":/icons/wall-brick.png";
    return  ":/icons/wall.png";
}

DynamicBody::DynamicBody(Scene* scene,const QString& name,QObject* parent) :
    Body(scene,name,parent),
    m_mass(1),
    m_moment(1),
    m_position(0,0),
    m_velocity(0,0),
    m_force(0,0),
    m_angle(0),
    m_angle_vel(0),
    m_torque(0),
    m_vel_limit(0),
    m_angle_vel_limit(0){

}

void DynamicBody::Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const {
    Body::Draw(canvas,painter,selected);
}
void DynamicBody::Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const {
    Body::Draw(canvas,painter,selected);
}

QString DynamicBody::iconFile() const {
    if (active())
        return ":/icons/car-red.png";
    return ":/icons/car.png";
}
