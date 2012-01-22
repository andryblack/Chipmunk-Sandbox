#include "rotateprimitivemarker.h"
#include "../canvas.h"
#include "../primitive.h"
#include "../commands/rotateprimitivecommand.h"

#include <cmath>

RotatePrimitiveMarker::RotatePrimitiveMarker(Primitive *primitive, QObject *parent) :
    PrimitiveMarker(parent), m_primitive( primitive )
{
    m_activated = false;
}


void RotatePrimitiveMarker::Draw( const Canvas* canvas , QPainter* painter ) const {
    canvas->Draw( this , painter );
}

bool RotatePrimitiveMarker::isPointInside( const QPointF& p) const {
    const QPointF delta =  (position()-p);
    return delta.x() * delta.x() + delta.y()*delta.y() <= radius() * radius();
}

qreal RotatePrimitiveMarker::radius() const {
    return 10.0f / m_primitive->sceneZoom();
}

QPointF RotatePrimitiveMarker::position() const {
    return m_primitive->position();
}

void RotatePrimitiveMarker::reset() {
    m_activated = false;
}

void RotatePrimitiveMarker::move(const QPointF& pos) {
    const QPointF delta =  (position()-pos);
    bool inside =  delta.x() * delta.x() + delta.y()*delta.y() <= radius() * radius();
    if (inside) {

    } else if (!m_activated) {
        m_activated = true;
        setStartPoint( pos );
        setEndPoint( pos );
        m_beginAngle = m_endAngle = primitiveAngle();
        m_delta_angle = atan2( delta.x(),-delta.y() );
    } else {
        setEndPoint( pos );
        qreal a = atan2( delta.x(),-delta.y() );
        rotatePrimitive( m_beginAngle+a-m_delta_angle );
        m_endAngle = primitiveAngle();
    }
}

bool RotatePrimitiveMarker::visible() const {
    const QSizeF sz = m_primitive->size();
    return sz.width() > radius() * 2 && sz.height() > radius() * 2;
}

Command* RotatePrimitiveMarker::generateCommand() {
    if (m_activated) {
        if (m_beginAngle!=m_endAngle) {
            return new RotatePrimitiveCommand(m_primitive,this,m_beginAngle,m_endAngle);
        }
    }
    return 0;
}
