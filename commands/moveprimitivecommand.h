#ifndef MOVEPRIMITIVECOMMAND_H
#define MOVEPRIMITIVECOMMAND_H

#include "../command.h"
#include <QPointF>

class Primitive;

class MovePrimitiveCommand : public Command
{
    Q_OBJECT
public:
    MovePrimitiveCommand(Primitive* p,const QPointF& from,
                         const QPointF& to,QObject* parent = 0);

    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;
private:
    Primitive* m_primitive;
    QPointF m_from;
    QPointF m_to;
};

#endif // MOVEPRIMITIVECOMMAND_H
