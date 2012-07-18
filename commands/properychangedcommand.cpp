#include "properychangedcommand.h"
#include <QRegExp>

ProperyChangedCommand::ProperyChangedCommand(QObject* obj,
                                             const char* prop,
                                             const QVariant &from,
                                             const QVariant &to,
                                             QObject *parent) :
    Command(parent), m_obj(obj),m_prop(prop),m_from(from),m_to(to)
{

}

void ProperyChangedCommand::Execute(Scene* /*scene*/)  {
    m_obj->setProperty(m_prop,m_to);
}

void ProperyChangedCommand::Undo(Scene* /*scene*/) {
    m_obj->setProperty(m_prop,m_from);
}

void ProperyChangedCommand::Redo(Scene* /*scene*/) {
    m_obj->setProperty(m_prop,m_to);
}

QString ProperyChangedCommand::text() const {
    QString str = m_prop;
    return QString("Change ")
            + str.at(0).toUpper()
            + str.mid(1).replace(QRegExp("([a-z])([A-Z])"), "\\1 \\2");
}
