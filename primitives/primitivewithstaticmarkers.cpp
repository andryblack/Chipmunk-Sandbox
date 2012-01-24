#include "primitivewithstaticmarkers.h"
#include "../primitive.h"
#include "../primitivemarker.h"

PrimitiveWithStaticMarkers::PrimitiveWithStaticMarkers(Scene *scene, QObject *parent) :
    Primitive(scene,parent)
{
}

void PrimitiveWithStaticMarkers::addMarker( PrimitiveMarker* marker ) {
    marker->setParent(this);
    m_markers.push_back(marker);
}

PrimitiveMarker* PrimitiveWithStaticMarkers::getMarkerAtPoint( const QPointF& pos ) {
    foreach( PrimitiveMarker* m, m_markers ) {
        if (m->isPointInside(pos))
            return m;
    }
    return 0;
}

void PrimitiveWithStaticMarkers::DrawMarkers( const Canvas* canvas, QPainter* painter ) const {
    foreach( PrimitiveMarker* m, m_markers ) {
        if (m->visible()) m->Draw( canvas, painter );
    }
}
