#include "boxprimitivemarker.h"
#include "../canvas.h"
#include "boxprimitive.h"
#include "../commands/movemarkercommand.h"

BoxPrimitiveMarker::BoxPrimitiveMarker(BoxPrimitive *primitive, QObject *parent) :
    PrimitiveMarker(parent), m_primitive(primitive)
{
}

QPointF BoxPrimitiveMarker::untransformPoint( const QPointF& pos ) const {
    QPointF p = pos;
    if (primitive()->angle()!=0) {
        QTransform transform;
        transform.translate(position().x(),position().y());
        transform.rotateRadians(-primitive()->angle());
        transform.translate(-position().x(),-position().y());
        p = transform.map(pos);
    }
    return p;
}

QPointF BoxPrimitiveMarker::transformPoint( const QPointF& pos ) const {
    QPointF p = pos;
    if (primitive()->angle()!=0) {
        QTransform transform;
        transform.translate(position().x(),position().y());
        transform.rotateRadians(primitive()->angle());
        transform.translate(-position().x(),-position().y());
        p = transform.map(pos);
    }
    return p;
}

bool BoxPrimitiveMarker::isPointInside( const QPointF& _p ) const {
    if (!visible()) return false;
    QPointF p = untransformPoint(_p);
    QPointF pos = position();
    qreal l = pos.x();
    qreal r = l;
    qreal t = pos.y();
    qreal b = t;
    PrimitiveMarkerXAlign x_align = xAlign();
    QSizeF sz = size();
    if (x_align==PrimitiveMarkerXAlign_Left) {
        r+=sz.width();
    } else if (x_align==PrimitiveMarkerXAlign_Right) {
        l-=sz.width();
    } else if (x_align==PrimitiveMarkerXAlign_Center) {
        l-=sz.width()/2;
        r+=sz.width()/2;
    }
    PrimitiveMarkerYAlign y_align = yAlign();
    if (y_align==PrimitiveMarkerYAlign_Top) {
        b+=sz.height();
    } else if (y_align==PrimitiveMarkerYAlign_Bottom) {
        t-=sz.height();
    } else if (y_align==PrimitiveMarkerYAlign_Center) {
        t-=sz.height()/2;
        b+=sz.height()/2;
    }
    return p.x() >= l && p.y() >= t && p.x() <= r && p.y() <= b;
}

void BoxPrimitiveMarker::move(const QPointF& pos) {
    if (!move_impl(primitive()->untransformPoint(pos))) return;
    setEndPoint(pos);
}

QPointF BoxPrimitiveMarker::position() const {
    return primitive()->transformPoint(position_impl());
}


Command* BoxPrimitiveMarker::generateCommand() {
    if ( activated() && startPoint()!=endPoint() ) {
        return new MoveMarkerCommand(primitive(),this,startPoint(),endPoint());
    }
    return 0;
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

QPointF BoxPrimitiveMarkerTL::position_impl() const {
    return primitive()->rect().topLeft();
}

bool BoxPrimitiveMarkerTL::move_impl(const QPointF& pos) {
    return primitive()->setTopLeft(pos);
}

QPointF BoxPrimitiveMarkerTR::position_impl() const {
    return primitive()->rect().topRight();
}

bool BoxPrimitiveMarkerTR::move_impl(const QPointF& pos) {
    return primitive()->setTopRight(pos);
}

QPointF BoxPrimitiveMarkerBL::position_impl() const {
    return primitive()->rect().bottomLeft();
}

bool BoxPrimitiveMarkerBL::move_impl(const QPointF& pos) {
    return primitive()->setBottomLeft(pos);
}

QPointF BoxPrimitiveMarkerBR::position_impl() const {
    return primitive()->rect().bottomRight();
}

bool BoxPrimitiveMarkerBR::move_impl(const QPointF& pos) {
    return primitive()->setBottomRight(pos);
}

bool BoxPrimitiveMarkerR::visible() const {
    return size().height() > width();
}

QSizeF BoxPrimitiveMarkerR::size() const {
    return haveHeight() ? QSizeF(width(),primitive()->rect().height()-width()*2) :
                          QSizeF(width(),primitive()->rect().height());
}

QPointF BoxPrimitiveMarkerR::position_impl() const {
    return QPointF(primitive()->rect().right(),primitive()->rect().top()+primitive()->rect().height()/2);
}

bool BoxPrimitiveMarkerR::move_impl(const QPointF& pos) {
    return primitive()->setRight(pos.x());
}

bool BoxPrimitiveMarkerL::visible() const {
    return size().height() > width();
}

QSizeF BoxPrimitiveMarkerL::size() const {
    return haveHeight() ? QSizeF(width(),primitive()->rect().height()-width()*2) :
                          QSizeF(width(),primitive()->rect().height());
}

QPointF BoxPrimitiveMarkerL::position_impl() const {
    return QPointF(primitive()->rect().left(),primitive()->rect().top()+primitive()->rect().height()/2);
}

bool BoxPrimitiveMarkerL::move_impl(const QPointF& pos) {
    return primitive()->setLeft(pos.x());
}

bool BoxPrimitiveMarkerT::visible() const {
    return size().width() > width();
}

QSizeF BoxPrimitiveMarkerT::size() const {
    return haveWidth() ? QSizeF(primitive()->rect().width()-width()*2,width()) :
                          QSizeF(primitive()->rect().width(),width());
}

QPointF BoxPrimitiveMarkerT::position_impl() const {
    return QPointF(primitive()->rect().left()+primitive()->rect().width()/2,primitive()->rect().top());
}

bool BoxPrimitiveMarkerT::move_impl(const QPointF& pos) {
    return primitive()->setTop(pos.y());
}

bool BoxPrimitiveMarkerB::visible() const {
    return size().width() > width();
}

QSizeF BoxPrimitiveMarkerB::size() const {
    return haveWidth() ? QSizeF(primitive()->rect().width()-width()*2,width()) :
                          QSizeF(primitive()->rect().width(),width());
}

QPointF BoxPrimitiveMarkerB::position_impl() const {
    return QPointF(primitive()->rect().left()+primitive()->rect().width()/2,primitive()->rect().bottom());
}

bool BoxPrimitiveMarkerB::move_impl(const QPointF& pos) {
    return primitive()->setBottom(pos.y());
}

RotateBoxPrimitiveMarker::RotateBoxPrimitiveMarker(BoxPrimitive *primitive, QObject *parent) :
    RotatePrimitiveMarker(primitive,parent) , m_box(primitive) {

}

qreal   RotateBoxPrimitiveMarker::primitiveAngle() const {
    return m_box->angle();
}

void RotateBoxPrimitiveMarker::rotatePrimitive( qreal a )  {
    m_box->rotate( a );
}

bool RotateBoxPrimitiveMarker::visible() const {
    const QSizeF sz = m_box->size();
    return sz.width() > radius() * 2 && sz.height() > radius() * 2;
}
