#include "polygonprimitive.h"
#include "../canvas.h"
#include "polygonprimitivemarker.h"
#include <cmath>

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

void PolygonPrimitive::DrawMarkers( const Canvas* canvas , QPainter* painter) const {
    foreach ( const PolygonPrimitiveMarker* m, m_corner_markers ) {
        canvas->Draw( m, painter );
    }

    if (m_points.size()>1) {
        QPointF prev = m_points.back();
        qreal w = 5.0 / sceneZoom();
        for (int i=0;i<m_points.size();i++) {
            QPointF d = m_points[i] - prev;
            qreal l = sqrt( d.x()*d.x() + d.y() * d.y() );
            l-=m_corner_markers[i]->width() * 2;
            if (l>w*3) {
                PolygonPrimitiveSubdivMarkerGhost ghost((m_points[i]+prev)/2,w);
                canvas->Draw( &ghost,painter );
            }
            prev = m_points[i];
        }
    }
}

PolygonPrimitiveMarker* PolygonPrimitive::getCornerMarkerAtPoint( const QPointF& pos, int andNot ) {
    for ( int i=0;i!=m_corner_markers.size();i++) {
        if (i!=andNot) {
            if (m_corner_markers[i]->isPointInside(pos)) return m_corner_markers[i];
        }
    }
    return 0;
}

PolygonPrimitiveMarker* PolygonPrimitive::getNearCornerMarkerAtPoint( const QPointF& pos,int index  ) {
    if (m_points.size()<3) return 0;
    int next = ( index + 1 ) % m_points.size();
    if (m_corner_markers[next]->isPointInside(pos))
        return m_corner_markers[next];
    int prev = ( index == 0 ) ? (m_corner_markers.size()-1) : index - 1;
    if (m_corner_markers[prev]->isPointInside(pos))
        return m_corner_markers[prev];
    return 0;
}

PrimitiveMarker* PolygonPrimitive::getMarkerAtPoint(const QPointF &pos) {
    PrimitiveMarker* p = getCornerMarkerAtPoint(pos,-1);
    if (p)
        return p;
    return 0;
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
    m_corner_markers.push_back(new PolygonPrimitiveMarker(this,m_points.size()-1));
    m_corner_markers.back()->setParent( this );
    updateText();
}

void    PolygonPrimitive::removeLast() {
    if (!m_points.empty()) {
        m_points.pop_back();
        delete m_corner_markers.back();
        m_corner_markers.pop_back();
    }
    updateText();
}

void    PolygonPrimitive::removePoint(int index) {
    if (index<m_points.size()) {
        m_points.remove(index);
        delete m_corner_markers[index];
        m_corner_markers.remove(index);
        for (int i=index;i<m_corner_markers.size();i++) {
            m_corner_markers[i]->setIndex(i);
        }
    }
    updateText();
}

void    PolygonPrimitive::addPoint( int index, const QPointF& pos) {
    if (index<=m_points.size()) {
        m_points.insert(index,pos);
        m_corner_markers.insert(index,new PolygonPrimitiveMarker(this,index));
        for (int i=index;i<m_corner_markers.size();i++) {
            m_corner_markers[i]->setIndex(i);
        }
    }
    updateText();
}

void    PolygonPrimitive::moveLastPoint( const QPointF& pos ) {
    if (!m_points.empty()) {
        m_points.back() = pos ;
        updateText();
    }
}

void    PolygonPrimitive::movePoint( int index, const QPointF& pos ) {
    if (index<m_points.size()) {
        m_points[index] = pos;
        updateText();
    }
}

void PolygonPrimitive::move( const QPointF& pos ) {
    QPointF delta = pos - position();
    m_points.translate(delta);
    updateText();
}


