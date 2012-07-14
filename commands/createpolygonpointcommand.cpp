#include "createpolygonpointcommand.h"
#include "../primitives/polygonprimitive.h"
#include "../scene.h"

CreatePolygonPointCommand::CreatePolygonPointCommand(PolygonPrimitive *p, int index, const QPointF &pos, QObject *parent) :
    Command(parent),m_primitive(p),m_index(index),m_pos(pos)
{
}

void CreatePolygonPointCommand::Execute(Scene* scene)  {
    m_primitive->addPoint( m_index, m_pos );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->update();
}

void CreatePolygonPointCommand::Undo(Scene* scene) {
    m_primitive->removePoint( m_index );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

void CreatePolygonPointCommand::Redo(Scene* scene)  {
    m_primitive->addPoint( m_index, m_pos );
    scene->setText(m_primitive->text());
    m_primitive->update();
    scene->setSelected(m_primitive);
}

QString CreatePolygonPointCommand::text() const {
    return QString("Create point to '%1'").arg(m_primitive->name());
}
