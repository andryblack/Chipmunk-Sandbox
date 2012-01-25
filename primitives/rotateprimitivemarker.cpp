#include "rotateprimitivemarker.h"
#include "../canvas.h"
#include "../primitive.h"
#include "../commands/rotateprimitivecommand.h"
#include "../scene.h"
#include <cmath>

RotatePrimitiveMarker::RotatePrimitiveMarker(Primitive *primitive, QObject *parent) :
    PrimitiveMarker(parent), m_primitive( primitive )
{
    m_rotate_activated = false;
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


void RotatePrimitiveMarker::move(const QPointF& pos) {
    const QPointF delta =  (position()-pos);
    bool inside =  delta.x() * delta.x() + delta.y()*delta.y() <= radius() * radius();
    if (inside) {

    } else if (!m_rotate_activated) {
        m_rotate_activated = true;
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

void RotatePrimitiveMarker::reset() {
    m_rotate_activated = false;
    PrimitiveMarker::reset();
}

void RotatePrimitiveMarker::complete() {
    if (activated() && m_rotate_activated) {
        if (m_beginAngle!=m_endAngle) {
            m_primitive->scene()->execCommand( new RotatePrimitiveCommand(m_primitive,this,m_beginAngle,m_endAngle) );
            PrimitiveMarker::complete();
            return;
        }
    }
    reset();
}
