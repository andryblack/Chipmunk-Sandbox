#include "rotateprimitivecommand.h"
#include "../primitives/rotateprimitivemarker.h"
#include "../scene.h"
#include "../primitive.h"

RotatePrimitiveCommand::RotatePrimitiveCommand(Primitive* p,RotatePrimitiveMarker* marker,qreal from,qreal to,QObject *parent) :
    Command(parent), m_primitive(p),m_marker(marker),m_from(from),m_to(to)
{
}


void RotatePrimitiveCommand::Execute(Scene* scene)  {
    m_marker->rotatePrimitive( m_to );
    scene->setText(m_primitive->text());
    scene->update();
}

void RotatePrimitiveCommand::Undo(Scene* scene) {
    m_marker->rotatePrimitive( m_from );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}

void RotatePrimitiveCommand::Redo(Scene* scene)  {
    m_marker->rotatePrimitive( m_to );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}

QString RotatePrimitiveCommand::text() const {
    return QString("Rotate '%1'").arg(m_primitive->name());
}
