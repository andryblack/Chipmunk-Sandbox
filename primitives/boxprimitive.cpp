#include "boxprimitive.h"
#include "../canvas.h"
#include "boxprimitivemarker.h"

BoxPrimitive::BoxPrimitive(Scene* scene,const QRectF& rect,QObject *parent) :
    Primitive(scene,parent), m_rect(rect)
{
    addMarker(new BoxPrimitiveMarkerTL(this));
    addMarker(new BoxPrimitiveMarkerTR(this));
    addMarker(new BoxPrimitiveMarkerBL(this));
    addMarker(new BoxPrimitiveMarkerBR(this));
    addMarker(new BoxPrimitiveMarkerL(this));
    addMarker(new BoxPrimitiveMarkerR(this));
    addMarker(new BoxPrimitiveMarkerT(this));
    addMarker(new BoxPrimitiveMarkerB(this));

    updateText();
}

void BoxPrimitive::updateText() {
    setText( QString("Box (%1,%2)-(%3x%4) [%5x%6]")
             .arg(m_rect.left(),0,'f',1)
             .arg(m_rect.top(),0,'f',1)
             .arg(m_rect.right(),0,'f',1)
             .arg(m_rect.bottom(),0,'f',1)
             .arg(m_rect.width(),0,'f',1)
             .arg(m_rect.height(),0,'f',1));
}

void BoxPrimitive::Draw( const Canvas* canvas , QPainter* painter) const {
    canvas->Draw(this,painter);
}

bool BoxPrimitive::isPointInside( const QPointF &p) const {
    return m_rect.contains( p );
}

QPointF BoxPrimitive::position() const {
    return m_rect.topLeft();
}

void BoxPrimitive::move( const QPointF& pos ) {
    m_rect.moveTopLeft(pos);
    updateText();
}

void BoxPrimitive::setSize( const QSizeF& size ) {
    m_rect.setSize(size);
    updateText();
}

void BoxPrimitive::setTopLeft( const QPointF& pos ) {
    if (pos.x()>=m_rect.right()) return;
    if (pos.y()>=m_rect.bottom()) return;
    m_rect.setTopLeft(pos);
    updateText();
}

void BoxPrimitive::setTopRight(const QPointF &pos) {
    if (pos.x()<=m_rect.left()) return;
    if (pos.y()>=m_rect.bottom()) return;
    m_rect.setTopRight(pos);
    updateText();
}

void BoxPrimitive::setBottomRight( const QPointF& pos ) {
    if (pos.x()<=m_rect.left()) return;
    if (pos.y()<=m_rect.top()) return;
    m_rect.setBottomRight(pos);
    updateText();
}

void BoxPrimitive::setBottomLeft(const QPointF &pos) {
    if (pos.x()>=m_rect.right()) return;
    if (pos.y()<=m_rect.top()) return;
    m_rect.setBottomLeft(pos);
    updateText();
}

void    BoxPrimitive::setRight( qreal r ) {
    if (r<=m_rect.left()) return;
    m_rect.setRight(r);
    updateText();
}

void    BoxPrimitive::setLeft(qreal l) {
    if (l>=m_rect.right()) return;
    m_rect.setLeft(l);
    updateText();
}

void    BoxPrimitive::setTop(qreal t) {
    if (t>=m_rect.bottom()) return;
    m_rect.setTop(t);
    updateText();
}

void    BoxPrimitive::setBottom(qreal b) {
    if (b<=m_rect.top()) return;
    m_rect.setBottom(b);
    updateText();
}
