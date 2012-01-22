#ifndef ROTATEPRIMITIVECOMMAND_H
#define ROTATEPRIMITIVECOMMAND_H

#include "../command.h"

class Primitive;
class RotatePrimitiveMarker;

class RotatePrimitiveCommand : public Command
{
    Q_OBJECT
public:
    explicit RotatePrimitiveCommand(Primitive* p,RotatePrimitiveMarker* marker,qreal from,qreal to,QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
signals:
    
public slots:
    
private:
    Primitive* m_primitive;
    RotatePrimitiveMarker* m_marker;
    qreal   m_from;
    qreal   m_to;
};

#endif // ROTATEPRIMITIVECOMMAND_H
