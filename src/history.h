#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QList>
#include <QString>

class Scene;
class Command;

class History : public QObject
{
    Q_OBJECT
public:
    History(QObject* parent=0);
    ~History();

    void clear();
    bool haveUnsavedChanges() const;
    void markSaved();

    bool undoAvaliable() const;
    QString undoText() const;
    bool redoAvaliable() const;
    QString redoText() const;

    void appendCommand( Command* cmd );

    void undo(Scene* scene);
    void redo(Scene* scene);
signals:
    void changed();
public slots:
private:
    QList<Command*> m_undo_list;
    QList<Command*> m_redo_list;
    Command*    m_last_saved;
};

#endif // HISTORY_H
