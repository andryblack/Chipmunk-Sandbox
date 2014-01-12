#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class Scene;

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = 0);
    
    virtual void Execute(Scene* scene) { Redo(scene); }
    virtual void Undo(Scene* scene) = 0;
    virtual void Redo(Scene* scene) = 0;

    virtual QString text() const = 0;
signals:
    
public slots:
private:
};

#endif // COMMAND_H
