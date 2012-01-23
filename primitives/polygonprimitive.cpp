#include "polygonprimitive.h"
#include "../canvas.h"

PolygonPrimitive::PolygonPrimitive(Scene *scene,const QPointF& pos,QObject *parent) :
    Primitive(scene,parent)
{
    setName("Polygon");
    addPoint(pos);
    updateText();
}


void PolygonPrimitive::updateText() {
    QString str = QString("Polygon (%1,%2) [pnts:%3]")
            .arg(position().x(),0,'f',1)
            .arg(position().y(),0,'f',1)
            .arg(m_points.size());
    setText( str );
}

void PolygonPrimitive::Draw( const Canvas* canvas , QPainter* painter) const {
    canvas->Draw( this, painter );
}

bool    PolygonPrimitive::insideStart(const QPointF& pos) const {
    if (m_points.empty()) return false;
    QPointF delta = pos - point(0);
    qreal len = delta.x()*delta.x()+delta.y()*delta.y();
    return len <= connectWidth() * connectWidth();
}

qreal PolygonPrimitive::connectWidth() const {
    return 10.0 / sceneZoom();
}

bool PolygonPrimitive::isPointInside( const QPointF& pos) const {
    return m_points.containsPoint(pos,Qt::OddEvenFill);
}


void    PolygonPrimitive::addPoint( const QPointF& pos ) {
    m_points.push_back( pos );
    updateText();
}

void    PolygonPrimitive::removeLast() {
    if (!m_points.empty())
        m_points.pop_back();
    updateText();
}

void    PolygonPrimitive::moveLastPoint( const QPointF& pos ) {
    if (!m_points.empty()) {
        m_points.back() = pos ;
        updateText();
    }
}

void PolygonPrimitive::move( const QPointF& pos ) {
    QPointF delta = pos - position();
    m_points.translate(delta);
    updateText();
}


