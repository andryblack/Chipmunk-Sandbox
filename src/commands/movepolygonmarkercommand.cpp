#include "movepolygonmarkercommand.h"
#include "../primitives/polygonprimitive.h"
#include "../scene.h"

MovePolygonMarkerCommand::MovePolygonMarkerCommand(PolygonPrimitive *p, int index, const QPointF &from, const QPointF &to, QObject *parent) :
    Command(parent),m_primitive( p ),m_index(index),m_from( from ), m_to ( to )
{
}


void MovePolygonMarkerCommand::Execute(Scene* scene)  {
    m_primitive->movePoint( m_index, m_to );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->update();
}

void MovePolygonMarkerCommand::Undo(Scene* scene) {
    m_primitive->movePoint( m_index, m_from );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

void MovePolygonMarkerCommand::Redo(Scene* scene)  {
    m_primitive->movePoint( m_index, m_to );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

QString MovePolygonMarkerCommand::text() const {
    return QString("Edit '%1'").arg(m_primitive->name());
}
