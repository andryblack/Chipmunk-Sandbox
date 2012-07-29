#include "circleprimitive.h"
#include "../canvas.h"
#include "circleprimitivemarker.h"

CirclePrimitive::CirclePrimitive(Body *body, const QPointF &pos, qreal r, QObject *parent) :
    PrimitiveWithStaticMarkers(body,parent),m_pos(pos),m_r(r)
{
    setName("Circle");
    addMarker(new CirclePrimitiveMarker(this));
    updateText();
}

void CirclePrimitive::updateText() {
    QString str = QString("Circle (%1,%2) [R:%3]")
            .arg(m_pos.x(),0,'f',1)
            .arg(m_pos.y(),0,'f',1)
            .arg(m_r,0,'f',1);
    setText( str );
}

void CirclePrimitive::setR( qreal r) {
    m_r = r;
    emit propertyChanged();
    updateText();
}

void   CirclePrimitive::setPosition( const QPointF& pos ) {
    move(pos);
}

void CirclePrimitive::move( const QPointF& pos ) {
    m_pos = pos;
    emit propertyChanged();
    updateText();
}

void CirclePrimitive::Draw( const Canvas* canvas , QPainter* painter) const {
    canvas->Draw(this,painter);
}

bool CirclePrimitive::isPointInside( const QPointF& pos) const {
    QPointF delta = pos-m_pos;
    return delta.x()*delta.x()+delta.y()*delta.y() <= m_r*m_r;
}
