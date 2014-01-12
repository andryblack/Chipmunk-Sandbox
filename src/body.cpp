#include "body.h"
#include "scene.h"
#include "primitive.h"
#include "canvas.h"
#include <QPainter>
#include <QTransform>

Body::Body(Scene* scene,const QString& name,QObject *parent) :
    SceneTreeItem(parent),m_scene(scene),
    m_position(0,0),
    m_angle(0)
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

Primitive* Body::getPrimitiveAtPoint(const QPointF &pnt_) {
    QPointF pnt = toLocal(pnt_);
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

bool Body::primitiveSelected( const Primitive* p) const {
    return m_scene->selected(p);
}

QPointF Body::toLocal( const QPointF& p ) const {
    QTransform transform;
    //transform.translate(-position().x(),-position().y());
    transform.rotateRadians(-angle());
    return transform.map(p-position());
}
QPointF Body::fromLocal( const QPointF& p ) const {
    QTransform transform;
    transform.translate(position().x(),position().y());
    transform.rotateRadians(-angle());
    return transform.map(p);
}

void Body::Draw( const Canvas* canvas , QPainter* painter ) const {
    qreal opc = painter->opacity();

    canvas->BeginDraw(this,painter);
    if (!active())
        painter->setOpacity(opc * 0.3);
    else {
        painter->save();
        painter->setPen(QColor(255,255,64,128));
        painter->drawLine(0,-10,0,10);
        painter->drawLine(-10,0,10,0);
        painter->restore();
    }
    QList<Primitive*> selected;
    foreach ( Primitive* p, m_primitives ) {
        if (m_scene->selected(p))
            selected.push_back(p);
        else
            p->Draw(canvas,painter);
    }
    if (!selected.empty()) {
        painter->setPen(QColor(255,64,64));
        painter->setBrush(QBrush(QColor(255,255,255,64)));

        foreach ( Primitive* p, selected ) {
            p->Draw(canvas,painter);
        }
        if ( selected.size()==1 && selected.front()==m_scene->selected() ) {
            painter->setBrush(QBrush(QColor(0,0,0,64)));
            painter->setPen(QColor(255,255,255,128));
            selected.front()->DrawMarkers(canvas,painter);
        }
    }
    canvas->EndDraw(this,painter);
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
    m_velocity(0,0),
    m_force(0,0),
    m_angle_vel(0),
    m_torque(0),
    m_vel_limit(0),
    m_angle_vel_limit(0){

}


void DynamicBody::Draw( const Canvas* canvas , QPainter* painter ) const {
    Body::Draw(canvas,painter);
}

QString DynamicBody::iconFile() const {
    if (active())
        return ":/icons/car-red.png";
    return ":/icons/car.png";
}
