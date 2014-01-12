#include "movebodycommand.h"
#include "../body.h"
#include "../scene.h"

MoveBodyCommand::MoveBodyCommand(Body *body, const QPointF &from, QPointF &to, QObject *parent) :
    Command(parent), m_body(body),m_from(from),m_to(to)
{
}

void MoveBodyCommand::Undo(Scene *scene)
{
    m_body->setPosition(m_from);
    scene->update();
}

void MoveBodyCommand::Redo(Scene *scene)
{
    m_body->setPosition(m_to);
    scene->update();
    scene->setSelected(m_body);
}

QString MoveBodyCommand::text() const
{
    return QString("Move '%1'").arg(m_body->name());
}
