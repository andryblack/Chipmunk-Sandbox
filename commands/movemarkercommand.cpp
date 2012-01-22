#include "movemarkercommand.h"
#include "../scene.h"
#include "../primitivemarker.h"
#include "../primitives/boxprimitive.h"

MoveMarkerCommand::MoveMarkerCommand(Primitive *p, PrimitiveMarker *m, const QPointF &from, const QPointF &to, QObject *parent) :
    Command(parent), m_primitive(p),m_marker(m),m_from(from),m_to(to)
{
}

void MoveMarkerCommand::Execute(Scene* scene)  {
    m_marker->move( m_to );
    scene->setText(m_primitive->text());
    scene->update();
}

void MoveMarkerCommand::Undo(Scene* scene) {
    m_marker->move( m_from );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}

void MoveMarkerCommand::Redo(Scene* scene)  {
    m_marker->move( m_to );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}

QString MoveMarkerCommand::text() const {
    return QString("Edit '%1'").arg(m_primitive->name());
}
