#include "circleprimitivemarker.h"
#include "circleprimitive.h"
#include "../canvas.h"
#include "../commands/movemarkercommand.h"

#include <cmath>

CirclePrimitiveMarker::CirclePrimitiveMarker(CirclePrimitive *primitive, QObject *parent) :
    PrimitiveMarker(parent), m_primitive( primitive )
{
}


void CirclePrimitiveMarker::Draw( const Canvas* canvas , QPainter* painter ) const {
    canvas->Draw( this, painter );
}

QPointF CirclePrimitiveMarker::position( ) const {
    return m_primitive->position() + QPointF(m_primitive->r(),0);
}
void    CirclePrimitiveMarker::move( const QPointF& pos ) {
    QPointF delta = pos - m_primitive->position();
    qreal len = sqrt( delta.x()*delta.x() + delta.y()*delta.y() );
    m_primitive->setR( len );
}

bool CirclePrimitiveMarker::CirclePrimitiveMarker::isPointInside( const QPointF& p) const {
    QPointF delta = p - m_primitive->position();
    qreal len2 =  delta.x()*delta.x() + delta.y()*delta.y();
    qreal r = m_primitive->r();
    qreal w = width();
    if ( haveWidth() ) {
        return len2 <= r * r && len2 >= (r-w)*(r-w) ;
    }
    return len2 <= (r+w)*(r+w) && len2 >= r*r;
}


qreal   CirclePrimitiveMarker::width() const {
    return 20.0 / primitive()->sceneZoom();
}

bool CirclePrimitiveMarker::haveWidth() const {
    return primitive()->r() > width() * 3;
}

Command*    CirclePrimitiveMarker::generateCommand() {
    if (startPoint()!=endPoint() ) {
        return new MoveMarkerCommand(m_primitive,this,startPoint(),endPoint());
    }
    return 0;
}