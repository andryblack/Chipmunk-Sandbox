#include "history.h"
#include "command.h"
#include "scene.h"

History::History(Scene* scene, QObject* object) : QObject(object),m_scene(scene)
{
}

History::~History() {
    foreach( Command* cmd,m_undo_list) {
        delete cmd;
    }
    foreach( Command* cmd,m_redo_list) {
        delete cmd;
    }
}

void History::setText(const QString &text) {
    m_scene->setText( text );
}

bool History::undoAvaliable() const {
    return !m_undo_list.empty();
}

bool History::redoAvaliable() const {
    return !m_redo_list.empty();
}


void History::appendCommand( Command* cmd, bool execute ) {
    m_undo_list.push_back(cmd);
    cmd->setParent(this);
    if (execute) {
        cmd->Execute(m_scene);
    }
    emit changed();
}

void History::undo() {
    if (m_undo_list.empty()) return;
    Command* cmd = m_undo_list.back();
    cmd->Undo(m_scene);
    m_undo_list.pop_back();
    m_redo_list.push_back(cmd);
    emit changed();
}

void History::redo() {
    if (m_redo_list.empty()) return;
    Command* cmd = m_redo_list.back();
    cmd->Redo(m_scene);
    m_redo_list.pop_back();
    m_undo_list.push_back(cmd);
    emit changed();
}
