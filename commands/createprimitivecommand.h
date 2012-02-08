#ifndef CREATEPRIMITIVECOMMAND_H
#define CREATEPRIMITIVECOMMAND_H

#include "../command.h"

class Primitive;
class Body;

class CreatePrimitiveCommand : public Command
{
    Q_OBJECT
public:
    CreatePrimitiveCommand(Body* body,Primitive* p,QObject* parent = 0);

    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
private:
    Body*       m_body;
    Primitive*  m_primitive;
};

#endif // CREATEPRIMITIVECOMMAND_H
