#include "moveprimitivecommand.h"
#include "../primitive.h"
#include "../scene.h"

MovePrimitiveCommand::MovePrimitiveCommand(Primitive *p, const QPointF &from, const QPointF &to, QObject *parent) : Command(parent),
    m_primitive( p ), m_from(from),m_to(to)
{
}


void MovePrimitiveCommand::Execute(Scene* scene)  {
    m_primitive->move( m_to );
    scene->setText(m_primitive->text());
    scene->update();
}

void MovePrimitiveCommand::Undo(Scene* scene) {
    m_primitive->move( m_from );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}

void MovePrimitiveCommand::Redo(Scene* scene)  {
    m_primitive->move( m_to );
    scene->setText(m_primitive->text());
    scene->setSelected(m_primitive);
}
