#ifndef REMOVEPOLYGONPOINTCOMMAND_H
#define REMOVEPOLYGONPOINTCOMMAND_H

#include <QPointF>
#include "../command.h"

class PolygonPrimitive;

class RemovePolygonPointCommand : public Command
{
    Q_OBJECT
public:
    explicit RemovePolygonPointCommand(PolygonPrimitive* p,int index, const QPointF& pos,QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
signals:
    
public slots:
private:
    PolygonPrimitive*   m_primitive;
    int m_index;
    QPointF m_pos;
};

#endif // REMOVEPOLYGONPOINTCOMMAND_H
