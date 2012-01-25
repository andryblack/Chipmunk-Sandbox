#include "polygonprimitivemarker.h"
#include "polygonprimitive.h"
#include "../canvas.h"
#include <cmath>
#include "../commands/movepolygonmarkercommand.h"
#include "../scene.h"

PolygonPrimitiveMarker::PolygonPrimitiveMarker(PolygonPrimitive *primitive,int index, QObject *parent) :
    PrimitiveMarker(parent), m_primitive( primitive ),m_index(index)
{
}

void PolygonPrimitiveMarker::Draw( const Canvas* canvas , QPainter* painter ) const {
    canvas->Draw( this, painter );
}

qreal PolygonPrimitiveMarker::width() const {
    return 10.0 / m_primitive->sceneZoom();
}

bool PolygonPrimitiveMarker::isPointInside( const QPointF& pos) const {
    QPointF delta = pos - position();
    qreal len = delta.x()*delta.x()+delta.y()*delta.y();
    return len <= width() * width();
}

QPointF PolygonPrimitiveMarker::position() const {
    return m_primitive->point(m_index);
}

void PolygonPrimitiveMarker::move(const QPointF& pos) {
    m_primitive->movePoint(m_index,pos);
    setEndPoint(pos);
}

void PolygonPrimitiveMarker::complete(Scene *scene) {
    if (!activated()) {
        return;
    }
    if (startPoint()==endPoint()) {
        reset();
        return;
    }
    scene->execCommand( new MovePolygonMarkerCommand(m_primitive,m_index,startPoint(),endPoint()) );
    PrimitiveMarker::complete(scene);
}
