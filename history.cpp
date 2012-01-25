#include "history.h"
#include "command.h"

History::History( QObject* object) : QObject(object)
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

bool History::undoAvaliable() const {
    return !m_undo_list.empty();
}

QString History::undoText() const {
    if (m_undo_list.empty()) return "Undo";
    return QString("Undo %1").arg( m_undo_list.back()->text() );
}

bool History::redoAvaliable() const {
    return !m_redo_list.empty();
}

QString History::redoText() const {
    if (m_redo_list.empty()) return "Redo";
    return QString("Redo %1").arg( m_redo_list.back()->text() );
}


void History::appendCommand( Command* cmd ) {
    /// @todo pack redo to one undo
    m_redo_list.clear();
    m_undo_list.push_back(cmd);
    cmd->setParent(this);
    emit changed();
}

void History::undo(Scene* scene) {
    if (m_undo_list.empty()) return;
    Command* cmd = m_undo_list.back();
    cmd->Undo(scene);
    m_undo_list.pop_back();
    m_redo_list.push_back(cmd);
    emit changed();
}

void History::redo(Scene* scene) {
    if (m_redo_list.empty()) return;
    Command* cmd = m_redo_list.back();
    cmd->Redo(scene);
    m_redo_list.pop_back();
    m_undo_list.push_back(cmd);
    emit changed();
}
