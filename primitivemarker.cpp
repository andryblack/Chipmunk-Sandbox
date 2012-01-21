#include "primitivemarker.h"
#include <QRectF>

PrimitiveMarker::PrimitiveMarker(QObject *parent) :
    QObject(parent)
{
}

bool PrimitiveMarker::isPointInside(const QPointF &p) const {
    if (!visible()) return false;
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
