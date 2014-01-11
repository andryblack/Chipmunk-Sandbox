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

QString PolygonPrimitive::pointsStr() const {
    QString res;
    for (int i=0;i<m_points.size();i++) {
        QPointF d = m_points[i];
        if (!res.isEmpty()) {
            res += ":";
        }
        res+=QString::number(d.x())+";"+QString::number(d.y());
    }
    return res;
}

void PolygonPrimitive::setPoints(const QString& str) {
    m_points.clear();
    m_corner_markers.clear();
    QStringList pts = str.split(':');
    foreach (QString s,pts) {
        QStringList pt = s.split(';');
        addPoint(QPointF(pt.first().toDouble(),pt.last().toDouble()));
    }
    emit propertyChanged();
    updateText();
    recalcDiagonals();
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
    emit propertyChanged();
    updateText();
}

void    PolygonPrimitive::removeLast() {
    if (!m_points.empty()) {
        m_points.pop_back();
        delete m_corner_markers.back();
        m_corner_markers.pop_back();
    }
    emit propertyChanged();
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
    emit propertyChanged();
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
    emit propertyChanged();
    updateText();
}

void    PolygonPrimitive::moveLastPoint( const QPointF& pos ) {
    if (!m_points.empty()) {
        m_points.back() = pos ;
        emit propertyChanged();
        updateText();
    }
}

void    PolygonPrimitive::movePoint( int index, const QPointF& pos ) {
    if (index<m_points.size()) {
        m_points[index] = pos;
        emit propertyChanged();
        updateText();
    }
}

void PolygonPrimitive::move( const QPointF& pos ) {
    QPointF delta = pos - position();
    m_points.translate(delta);
    emit propertyChanged();
    updateText();
}

/// Mark Keil's Algorithm

static inline int wrap(const int a, const int b) {
    return a < 0 ? a % b + b : a % b;
}

static inline const QPointF& at( const QPolygonF& polygon, int i) {
    return polygon[wrap(i, polygon.size())];
}

static inline bool eq(const qreal a, const qreal b) {
    return abs(a - b) <= 1e-8;
}

static inline qreal sqdist(const QPointF &a, const QPointF &b) {
    qreal dx = b.x() - a.x();
    qreal dy = b.y() - a.y();
    return dx * dx + dy * dy;
}

static QPointF intersection(const QPointF &p1, const QPointF &p2, const QPointF &q1, const QPointF &q2) {
    QPointF i;
    qreal a1, b1, c1, a2, b2, c2, det;
    a1 = p2.y() - p1.y();
    b1 = p1.x() - p2.x();
    c1 = a1 * p1.x() + b1 * p1.y();
    a2 = q2.y() - q1.y();
    b2 = q1.x() - q2.x();
    c2 = a2 * q1.x() + b2 * q1.y();
    det = a1 * b2 - a2*b1;
    if (!eq(det, 0)) { // lines are not parallel
        i.setX( (b2 * c1 - b1 * c2) / det );
        i.setY( (a1 * c2 - a2 * c1) / det );
    }
    return i;
}



static inline qreal area(const QPointF &a, const QPointF &b, const QPointF &c) {
    return (((b.x() - a.x())*(c.y() - a.y()))-((c.x() - a.x())*(b.y() - a.y())));
}

static inline bool left(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) > 0;
}

static inline bool leftOn(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) >= 0;
}

static inline bool right(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) < 0;
}

static inline bool rightOn(const QPointF &a, const QPointF &b, const QPointF &c) {
    return area(a, b, c) <= 0;
}

static inline bool isReflex(const QPolygonF& p,int i) {
    return right(at(p,i-1), at(p,i), at(p,i+1));
}

static bool can_see( const QPolygonF& polygon, int a, int b ) {

    QPointF p;
    qreal dist;

    if (leftOn(at(polygon,a + 1), at(polygon,a), at(polygon,b))
            && rightOn(at(polygon,a - 1), at(polygon,a), at(polygon,b))) {
        return false;
    }
    dist = sqdist(at(polygon,a), at(polygon,b));
    for (int i = 0; i < polygon.size(); ++i) { // for each edge
        if ((i + 1) % polygon.size() == a || i == a) // ignore incident edges
            continue;
        if (leftOn(at(polygon,a), at(polygon,b), at(polygon,i + 1))
                && rightOn(at(polygon,a), at(polygon,b), at(polygon,i))) { // if diag intersects an edge
            p = intersection(at(polygon,a), at(polygon,b), at(polygon,i), at(polygon,i + 1));
            if (sqdist(at(polygon,a), p) < dist) { // if edge is blocking visibility to b
                return false;
            }
        }
    }

    return true;
}


static void split( const QPolygonF& polygon, int p1, int p2, QPolygonF& left, QPolygonF& right) {
    p1 = wrap(p1,polygon.size());
    int end1 = wrap(p2+1,polygon.size());
    for (int i=p1;i!=end1;) {
        left.push_back(polygon[i]);
        i = wrap(i+1,polygon.size());
    }
    p2 = wrap(p2,polygon.size());
    int end2 = wrap(p1+1,polygon.size());
    for (int i=p2;i!=end2;) {
        right.push_back(polygon[i]);
        i = wrap(i+1,polygon.size());
    }
}


static PolygonPrimitive::Diagonals merge( const PolygonPrimitive::Diagonals& a, const PolygonPrimitive::Diagonals& b,const QPolygonF& polygon,int i,int j) {
    PolygonPrimitive::Diagonals res;
    res.reserve(a.size()+b.size());
    foreach ( PolygonPrimitive::Diagonal d, a ) {
        d.first = wrap( d.first+i ,polygon.size() );
        d.second = wrap( d.second+i ,polygon.size() );
        res.push_back(d);
    }
    foreach ( PolygonPrimitive::Diagonal d, b ) {
        d.first = wrap( d.first+j ,polygon.size() );
        d.second = wrap( d.second+j ,polygon.size() );
        res.push_back(d);
    }
    return res;
}

static PolygonPrimitive::Diagonals decomp( const QPolygonF& polygon ) {
    PolygonPrimitive::Diagonals tmp;
    if (polygon.size()<4) return tmp;
    PolygonPrimitive::Diagonals min;

    QPolygonF left;
    QPolygonF right;

    left.reserve(polygon.size());
    right.reserve(polygon.size());

    int ndiags = polygon.size();
    for (int i=0;i<polygon.size();++i) {
        if ( isReflex(polygon,i) ) {
            for (int j=0;j<polygon.size();j++) {
                if (can_see(polygon,i,j)) {
                    left.clear();
                    right.clear();
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
    if (!left(at(m_points,br - 1), at(m_points,br), at(m_points,br + 1))) {
        std::reverse( m_points.begin(),m_points.end() );
    }

    recalcDiagonals();
}

void PolygonPrimitive::recalcDiagonals() {
    m_diagonals.clear();
    if (m_points.size()<3) return;
    m_diagonals = decomp(m_points);
}

