#include "boxprimitivemarker.h"
#include "../canvas.h"
#include "boxprimitive.h"

BoxPrimitiveMarker::BoxPrimitiveMarker(BoxPrimitive *primitive, QObject *parent) :
    PrimitiveMarker(parent), m_primitive(primitive)
{
}

qreal   BoxPrimitiveMarker::width() const {
    return 20.0 / primitive()->sceneZoom();
}

bool BoxPrimitiveMarker::haveWidth() const {
    return primitive()->rect().width() > width() * 3;
}

bool BoxPrimitiveMarker::haveHeight() const {
    return primitive()->rect().height() > width() * 3;
}

void BoxPrimitiveMarker::Draw( const Canvas* canvas , QPainter* painter ) const {
    canvas->Draw(this,painter);
}

QPointF BoxPrimitiveMarkerTL::position() const {
    return primitive()->position();
}

void BoxPrimitiveMarkerTL::move(const QPointF& pos) {
    primitive()->setTopLeft(pos);
}

QPointF BoxPrimitiveMarkerTR::position() const {
    return primitive()->rect().topRight();
}

void BoxPrimitiveMarkerTR::move(const QPointF& pos) {
    primitive()->setTopRight(pos);
}

QPointF BoxPrimitiveMarkerBL::position() const {
    return primitive()->rect().bottomLeft();
}

void BoxPrimitiveMarkerBL::move(const QPointF& pos) {
    primitive()->setBottomLeft(pos);
}

QPointF BoxPrimitiveMarkerBR::position() const {
    return primitive()->rect().bottomRight();
}

void BoxPrimitiveMarkerBR::move(const QPointF& pos) {
    primitive()->setBottomRight(pos);
}

bool BoxPrimitiveMarkerR::visible() const {
    return size().height() > width();
}

QSizeF BoxPrimitiveMarkerR::size() const {
    return haveHeight() ? QSizeF(width(),primitive()->rect().height()-width()*2) :
                          QSizeF(width(),primitive()->rect().height());
}

QPointF BoxPrimitiveMarkerR::position() const {
    return QPointF(primitive()->rect().right(),primitive()->rect().top()+primitive()->rect().height()/2);
}

void BoxPrimitiveMarkerR::move(const QPointF& pos) {
    primitive()->setRight(pos.x());
}

bool BoxPrimitiveMarkerL::visible() const {
    return size().height() > width();
}

QSizeF BoxPrimitiveMarkerL::size() const {
    return haveHeight() ? QSizeF(width(),primitive()->rect().height()-width()*2) :
                          QSizeF(width(),primitive()->rect().height());
}

QPointF BoxPrimitiveMarkerL::position() const {
    return QPointF(primitive()->rect().left(),primitive()->rect().top()+primitive()->rect().height()/2);
}

void BoxPrimitiveMarkerL::move(const QPointF& pos) {
    primitive()->setLeft(pos.x());
}

bool BoxPrimitiveMarkerT::visible() const {
    return size().width() > width();
}

QSizeF BoxPrimitiveMarkerT::size() const {
    return haveWidth() ? QSizeF(primitive()->rect().width()-width()*2,width()) :
                          QSizeF(primitive()->rect().width(),width());
}

QPointF BoxPrimitiveMarkerT::position() const {
    return QPointF(primitive()->rect().left()+primitive()->rect().width()/2,primitive()->rect().top());
}

void BoxPrimitiveMarkerT::move(const QPointF& pos) {
    primitive()->setTop(pos.y());
}

bool BoxPrimitiveMarkerB::visible() const {
    return size().width() > width();
}

QSizeF BoxPrimitiveMarkerB::size() const {
    return haveWidth() ? QSizeF(primitive()->rect().width()-width()*2,width()) :
                          QSizeF(primitive()->rect().width(),width());
}

QPointF BoxPrimitiveMarkerB::position() const {
    return QPointF(primitive()->rect().left()+primitive()->rect().width()/2,primitive()->rect().bottom());
}

void BoxPrimitiveMarkerB::move(const QPointF& pos) {
    primitive()->setBottom(pos.y());
}
