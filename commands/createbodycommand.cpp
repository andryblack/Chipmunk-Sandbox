#include "createbodycommand.h"
#include "../body.h"
#include "../scene.h"

CreateBodyCommand::CreateBodyCommand(Body* b,QObject *parent) :
    Command(parent), m_body(b)
{
}

void CreateBodyCommand::Execute(Scene* scene) {
    scene->addBody(m_body);
    //scene->setText(m_body->text());
    //m_primitive->update();
}

void CreateBodyCommand::Undo(Scene* scene) {
    m_body->setParent(this);
    scene->removeBody(m_body);
    if (scene->activeBody()==m_body)
        scene->setActiveBody(0);
    scene->setText("");
}

void CreateBodyCommand::Redo(Scene* scene) {
    scene->addBody(m_body);
}

QString CreateBodyCommand::text() const {
    return QString("Create '%1'").arg(m_body->name());
}
