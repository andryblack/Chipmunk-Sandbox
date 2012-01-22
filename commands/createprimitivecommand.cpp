#include "createprimitivecommand.h"
#include "../scene.h"
#include "../primitive.h"

CreatePrimitiveCommand::CreatePrimitiveCommand(Primitive* p,QObject* parent) : Command(parent), m_primitive(p)
{
}


void CreatePrimitiveCommand::Execute(Scene* scene) {
    scene->appendPrimitive(m_primitive);
    scene->setText(m_primitive->text());
}

void CreatePrimitiveCommand::Undo(Scene* scene) {
    m_primitive->setParent(this);
    scene->clearSelection();
    scene->removePrimitive(m_primitive);
    scene->setText("");
}

void CreatePrimitiveCommand::Redo(Scene* scene) {
    scene->appendPrimitive(m_primitive);
    scene->setText(m_primitive->text());
}

QString CreatePrimitiveCommand::text() const {
    return QString("Create '%1'").arg(m_primitive->name());
}
