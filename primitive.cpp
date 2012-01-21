#include "primitive.h"
#include "canvas.h"
#include "primitivemarker.h"
#include "scene.h"

Primitive::Primitive(Scene* scene,QObject *parent) :
    QObject(parent), m_scene(scene)
{
}

qreal   Primitive::sceneZoom() const {
    return m_scene->zoom();
}

void Primitive::addMarker( PrimitiveMarker* marker ) {
    marker->setParent(this);
    m_markers.push_back(marker);
}

PrimitiveMarker* Primitive::getMarkerAtPoint( const QPointF& pos ) {
    foreach( PrimitiveMarker* m, m_markers ) {
        if (m->isPointInside(pos))
            return m;
    }
    return 0;
}

void Primitive::DrawMarkers( const Canvas* canvas, QPainter* painter ) const {
    foreach( PrimitiveMarker* m, m_markers ) {
        if (m->visible()) m->Draw( canvas, painter );
    }
}

