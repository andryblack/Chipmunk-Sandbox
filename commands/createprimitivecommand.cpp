#include "createprimitivecommand.h"
#include "../scene.h"
#include "../primitive.h"
#include "../body.h"

CreatePrimitiveCommand::CreatePrimitiveCommand(Body *body,Primitive *p, QObject *parent) : Command(parent),m_body(body),m_primitive(p)
{
}


void CreatePrimitiveCommand::Execute(Scene* scene) {
    m_body->addPrimitive(m_primitive);
    scene->setText(m_primitive->text());
    m_primitive->update();
}

void CreatePrimitiveCommand::Undo(Scene* scene) {
    m_primitive->setParent(this);
    m_body->removePrimitive(m_primitive);
    if (scene->selected()==m_primitive)
        scene->clearSelection();
    scene->setText("");
    m_primitive->update();
}

void CreatePrimitiveCommand::Redo(Scene* scene) {
    m_body->addPrimitive(m_primitive);
    scene->setText(m_primitive->text());
    m_primitive->update();
}

QString CreatePrimitiveCommand::text() const {
    return QString("Create '%1'").arg(m_primitive->name());
}
