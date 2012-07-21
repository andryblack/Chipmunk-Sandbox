#include "removepolygonpointcommand.h"
#include "../primitives/polygonprimitive.h"
#include "../scene.h"

RemovePolygonPointCommand::RemovePolygonPointCommand(PolygonPrimitive *p, int index, const QPointF &pos, QObject *parent) :
    Command(parent),m_primitive(p),m_index(index),m_pos(pos)
{
}


void RemovePolygonPointCommand::Execute(Scene* scene)  {
    m_primitive->removePoint( m_index );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->update();
}

void RemovePolygonPointCommand::Undo(Scene* scene) {
    m_primitive->addPoint( m_index, m_pos );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

void RemovePolygonPointCommand::Redo(Scene* scene)  {
    m_primitive->removePoint( m_index );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

QString RemovePolygonPointCommand::text() const {
    return QString("Delete point from '%1'").arg(m_primitive->name());
}
