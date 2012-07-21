#include "polygonprimitive.h"
#include "../canvas.h"
#include "polygonprimitivemarker.h"
#include "../body.h"
#include "../commands/createpolygonpointcommand.h"
#include <cmath>
#include <QtAlgorithms>

PolygonPrimitive::PolygonPrimitive(Body *body,const QPointF& pos,QObject *parent) :
    Primitive(body,parent)
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

    if (m_points.size()>1) {
        QPointF prev = m_points.back();
        qreal w = 5.0 / sceneZoom();
        for (int i=0;i<m_points.size();i++) {
            QPointF d = m_points[i] - prev;
            qreal l = sqrt( d.x()*d.x() + d.y() * d.y() );
            l-=m_corner_markers[i]->width() * 2;
            if (l>w*3) {
                QPointF splitPos = (m_points[i]+prev)/2;
                d = splitPos - pos;
                l = sqrt( d.x()*d.x() + d.y() * d.y() );
                if (l<w) {
                    int indx = i;
                    body()->execCommand( new CreatePolygonPointCommand(this,indx,splitPos));
                    return getCornerMarkerAtPoint(pos,-1);
                }
            }
            prev = m_points[i];
        }
    }

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

/// Mark Keil's Algorithm
static inline int ring( const QPolygonF& polygon, int i) {
    if (i<0) return polygon.size()+i;
    return i % polygon.size();
}

static bool intersect(const QPointF& start1, const QPointF& end1, const QPointF& start2, const QPointF& end2)
    {
        QPointF dir1 = end1 - start1;
        QPointF dir2 = end2 - start2;

        //считаем уравнения прямых проходящих через отрезки
        qreal a1 = -dir1.y();
        qreal b1 = +dir1.x();
        qreal d1 = -(a1*start1.x() + b1*start1.y());

        qreal a2 = -dir2.y();
        qreal b2 = +dir2.x();
        qreal d2 = -(a2*start2.x() + b2*start2.y());

        //подставляем концы отрезков, для выяснения в каких полуплоскотях они
        qreal seg1_line2_start = a2*start1.x() + b2*start1.y() + d2;
        qreal seg1_line2_end = a2*end1.x() + b2*end1.y() + d2;

        qreal seg2_line1_start = a1*start2.x() + b1*start2.y() + d1;
        qreal seg2_line1_end = a1*end2.x() + b1*end2.y() + d1;

        //если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет.
        if (seg1_line2_start * seg1_line2_end >= 0 || seg2_line1_start * seg2_line1_end >= 0)
            return false;


        return true;
    }

static qreal area(const QPointF &a, const QPointF &b, const QPointF &c) {
    return (((b.x() - a.x())*(c.y() - a.y()))-((c.x() - a.x())*(b.y() - a.y())));
}

static bool leftOn(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) >= 0;
}

static bool rightOn(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) <= 0;
}

static bool isReflex(const QPolygonF& p,int i) {
    return area(p[ring(p,i-1)], p[i], p[ring(p,i+1)])<0;
}

static bool can_see( const QPolygonF& polygon, int p1, int p2 ) {
    QPointF a1 = polygon[p1];
    QPointF a2 = polygon[p2];

    if (leftOn(polygon[ring(polygon,p1 + 1)], a1, a2) && leftOn(polygon[ring(polygon,p1 - 1)], a1, a2)) {
        return false;
    }
    if (rightOn(polygon[ring(polygon,p1 + 1)], a1, a2) && rightOn(polygon[ring(polygon,p1 - 1)], a1, a2)) {
        return false;
    }
    for (int i=0;i<polygon.size();++i) {
        if (i==p1) continue;
        if (i==p2) continue;
        int i1 = ring(polygon,i+1);
        if (i1==p1) continue;
        if (i1==p2) continue;

        QPointF b1 = polygon[i];
        QPointF b2 = polygon[i1];
        if (intersect(a1,a2,b1,b2))
            return false;
    }
    return true;
}


static void split( const QPolygonF& polygon, int p1, int p2, QPolygonF& left, QPolygonF& right) {
    for (int i=p1;i!=p2;) {
        left.push_back(polygon[i]);
        i = ring(polygon,i+1);
    }
    for (int i=p2;i!=p1;) {
        right.push_back(polygon[i]);
        i = ring(polygon,i+1);
    }
}


static PolygonPrimitive::Diagonals merge( const PolygonPrimitive::Diagonals& a, const PolygonPrimitive::Diagonals& b,const QPolygonF& polygon,int i,int j) {
    PolygonPrimitive::Diagonals res;
    foreach ( PolygonPrimitive::Diagonal d, a ) {
        d.first = ring( polygon, d.first+i );
        d.second = ring( polygon, d.second+i );
        res.push_back(d);
    }
    foreach ( PolygonPrimitive::Diagonal d, b ) {
        d.first = ring( polygon, d.first+j );
        d.second = ring( polygon, d.second+j );
        res.push_back(d);
    }
    return res;
}

static PolygonPrimitive::Diagonals decomp( const QPolygonF& polygon ) {
    PolygonPrimitive::Diagonals tmp;
    if (polygon.size()<4) return tmp;
    PolygonPrimitive::Diagonals min;
    int ndiags = polygon.size();
    for (int i=0;i<polygon.size();++i) {
        if ( isReflex(polygon,i) ) {
            for (int j=0;j<polygon.size();j++) {
                if (i!=j && (i!=ring(polygon,j+1)) && (j!=ring(polygon,i+1))) {
                    if (can_see(polygon,i,j)) {
                        QPolygonF left;
                        QPolygonF right;
                        split( polygon,i,j,left,right);
                        tmp = merge( decomp( left ) , decomp( right ),polygon,i,j);
                        if (tmp.size()<ndiags) {
                            min = tmp;
                            ndiags = tmp.size();
                            min.push_back( PolygonPrimitive::Diagonal(i,j) );
                        }
                    }
                }
            }
        }
    }
    return min;
}

void PolygonPrimitive::update() {
    /// make CCW
    int br = 0;

    if (m_points.size()<3) return;

    // find bottom right point
    for (int i = 1; i < m_points.size(); ++i) {
        if (m_points[i].y() < m_points[br].y() || (m_points[i].y() == m_points[br].y() && m_points[i].x() > m_points[br].x())) {
            br = i;
        }
    }

    // reverse poly if clockwise
    if (area(m_points[ring(m_points,br - 1)], m_points[br], m_points[ring(m_points,br + 1)])<0) {
        std::reverse( m_points.begin(),m_points.end() );
    }

    recalcDiagonals();
}

void PolygonPrimitive::recalcDiagonals() {
    m_diagonals.clear();
    if (m_points.size()<3) return;
    m_diagonals = decomp(m_points);
}

