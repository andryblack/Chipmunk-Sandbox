#include "primitivemarker.h"
#include <QRectF>

PrimitiveMarker::PrimitiveMarker(QObject *parent) :
    QObject(parent),m_activated(false)
{
}

void PrimitiveMarker::activate( ) {
    if (activated()) {
        reset();
    }
    setStartPoint(position());
    m_activated = true;
 }

void PrimitiveMarker::reset() {
    if (m_activated) {
        move(startPoint());
    }
    m_activated = false;
}

void PrimitiveMarker::complete(Scene * /*scene*/) {
    m_activated = false;
}
