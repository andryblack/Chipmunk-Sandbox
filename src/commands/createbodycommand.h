#ifndef CREATEBODYCOMMAND_H
#define CREATEBODYCOMMAND_H

#include "../command.h"

class Body;

class CreateBodyCommand : public Command
{
    Q_OBJECT
public:
    explicit CreateBodyCommand(Body *b, QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
signals:
    
public slots:
private:
    Body*   m_body;
};

#endif // CREATEBODYCOMMAND_H
