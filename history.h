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
    History(Scene* scene,QObject* parent=0);
    ~History();

    bool undoAvaliable() const;
    QString undoText() const;
    bool redoAvaliable() const;
    QString redoText() const;

    void appendCommand( Command* cmd, bool execute );

    Scene* scene() { return m_scene;}

    void   setText( const QString& text );
signals:
    void changed();
public slots:
    void undo();
    void redo();
private:
    Scene*  m_scene;
    QList<Command*> m_undo_list;
    QList<Command*> m_redo_list;
};

#endif // HISTORY_H
