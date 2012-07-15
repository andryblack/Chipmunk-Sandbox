#include "boxprimitive.h"
#include "../canvas.h"
#include "boxprimitivemarker.h"

#include <QTransform>

#include <cmath>

BoxPrimitive::BoxPrimitive(Body *body,const QRectF& rect,QObject *parent) :
    PrimitiveWithStaticMarkers(body,parent), m_rect(rect), m_angle( 0 )
{
    setName("Box");

    addMarker(new BoxPrimitiveMarkerTL(this));
    addMarker(new BoxPrimitiveMarkerTR(this));
    addMarker(new BoxPrimitiveMarkerBL(this));
    addMarker(new BoxPrimitiveMarkerBR(this));
    addMarker(new BoxPrimitiveMarkerL(this));
    addMarker(new BoxPrimitiveMarkerR(this));
    addMarker(new BoxPrimitiveMarkerT(this));
    addMarker(new BoxPrimitiveMarkerB(this));
    addMarker(new RotateBoxPrimitiveMarker(this));

    updateText();
}

void BoxPrimitive::updateText() {
    QString str = QString("Box (%1,%2) [%3x%4]")
            .arg(m_rect.center().x(),0,'f',1)
            .arg(m_rect.center().y(),0,'f',1)
            .arg(m_rect.width(),0,'f',1)
            .arg(m_rect.height(),0,'f',1);
    if (angle()!=0) {
        str+= QString(",%1").arg(angle()*180.0/M_PI,0,'f',2);
    }
    setText( str );
}

void BoxPrimitive::Draw( const Canvas* canvas , QPainter* painter) const {
    canvas->Draw(this,painter);
}

QPointF BoxPrimitive::untransformPoint( const QPointF& pos ) const {
    QPointF p = pos;
    if (angle()!=0) {
        QTransform transform;
        transform.translate(position().x(),position().y());
        transform.rotateRadians(-angle());
        transform.translate(-position().x(),-position().y());
        p = transform.map(pos);
    }
    return p;
}

QPointF BoxPrimitive::transformPoint( const QPointF& pos ) const {
    QPointF p = pos;
    if (angle()!=0) {
        QTransform transform;
        transform.translate(position().x(),position().y());
        transform.rotateRadians(angle());
        transform.translate(-position().x(),-position().y());
        p = transform.map(pos);
    }
    return p;
}

bool BoxPrimitive::isPointInside( const QPointF &pos) const {
    return m_rect.contains( untransformPoint(pos) );
}

QPointF BoxPrimitive::position() const {
    return m_rect.center();
}

QSizeF BoxPrimitive::size() const {
    return m_rect.size();
}

void BoxPrimitive::move( const QPointF& pos ) {
    m_rect.moveCenter(pos);
    emit propertyChanged();
    updateText();
}

void BoxPrimitive::setSize( const QSizeF& size ) {
    m_rect.setSize(size);
    emit propertyChanged();
    updateText();
}

bool BoxPrimitive::setTopLeft( const QPointF& pos ) {
    if (pos.x()>=m_rect.right()) return false;
    if (pos.y()>=m_rect.bottom()) return false;
    QPointF d = pos - m_rect.topLeft();
    QPointF p = position();
    QPointF newPos = transformPoint(QPointF(p.x()+d.x()/2,p.y()+d.y()/2));
    m_rect.setTopLeft(pos);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool BoxPrimitive::setTopRight(const QPointF &pos) {
    if (pos.x()<=m_rect.left()) return false;
    if (pos.y()>=m_rect.bottom()) return false;
    QPointF d = pos - m_rect.topRight();
    QPointF p = position();
    QPointF newPos = transformPoint(QPointF(p.x()+d.x()/2,p.y()+d.y()/2));
    m_rect.setTopRight(pos);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool BoxPrimitive::setBottomRight( const QPointF& pos ) {
    if (pos.x()<=m_rect.left()) return false;
    if (pos.y()<=m_rect.top()) return false;
    QPointF d = pos - m_rect.bottomRight();
    QPointF p = position();
    QPointF newPos = transformPoint(QPointF(p.x()+d.x()/2,p.y()+d.y()/2));
    m_rect.setBottomRight(pos);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool BoxPrimitive::setBottomLeft(const QPointF &pos) {
    if (pos.x()>=m_rect.right()) return false;
    if (pos.y()<=m_rect.top()) return false;
    QPointF d = pos - m_rect.bottomLeft();
    QPointF p = position();
    QPointF newPos = transformPoint(QPointF(p.x()+d.x()/2,p.y()+d.y()/2));
    m_rect.setBottomLeft(pos);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool    BoxPrimitive::setRight( qreal r ) {
    if (r<=m_rect.left()) return false;
    qreal d = r - m_rect.right();
    QPointF pos = position();
    QPointF newPos = transformPoint(QPointF(pos.x()+d/2,pos.y()));
    m_rect.setRight(r);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool    BoxPrimitive::setLeft(qreal l) {
    if (l>=m_rect.right()) return false;
    qreal d = l - m_rect.left();
    QPointF pos = position();
    QPointF newPos = transformPoint(QPointF(pos.x()+d/2,pos.y()));
    m_rect.setLeft(l);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool    BoxPrimitive::setTop(qreal t) {
    if (t>=m_rect.bottom()) return false;
    qreal d = t - m_rect.top();
    QPointF pos = position();
    QPointF newPos = transformPoint(QPointF(pos.x(),pos.y()+d/2));
    m_rect.setTop(t);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

bool    BoxPrimitive::setBottom(qreal b) {
    if (b<=m_rect.top()) return false;
    qreal d = b - m_rect.bottom();
    QPointF pos = position();
    QPointF newPos = transformPoint(QPointF(pos.x(),pos.y()+d/2));
    m_rect.setBottom(b);
    m_rect.moveCenter(newPos);
    updateText();
    return true;
}

void    BoxPrimitive::rotate( qreal a ) {
    m_angle = a;
    updateText();
}
