#include "body.h"
#include "scene.h"
#include "primitive.h"

Body::Body(Scene* scene,const QString& name,QObject *parent) :
    SceneTreeItem(parent),m_scene(scene),m_name(name)
{
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

void Body::select( bool s) {
    if (s) {

    } else {

    }
}

void Body::selectPrimitive( Primitive* p , bool select) {
    if (select) {
        m_scene->addSelected(p);
    } else {
        m_scene->removeSelected(p);
    }
}

StaticBody::StaticBody(Scene* scene,const QString &name, QObject *parent) :
    Body(scene,name,parent) {

}

void StaticBody::Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const {
    foreach ( Primitive* p, m_primitives ) {
        if (selected.indexOf(p)==-1) {
            p->Draw( canvas, painter );
        }
    }
}

void StaticBody::Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const {
    selected->Draw(canvas,painter);
}
