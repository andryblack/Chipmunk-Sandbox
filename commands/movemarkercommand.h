#ifndef MOVEMARKERCOMMAND_H
#define MOVEMARKERCOMMAND_H

#include "../command.h"

#include <QPointF>

class Primitive;
class PrimitiveMarker;

class MoveMarkerCommand : public Command
{
    Q_OBJECT
public:
    explicit MoveMarkerCommand(Primitive* p,
                               PrimitiveMarker* m,
                               const QPointF& from,
                               const QPointF& to,QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

signals:
    
public slots:
    
private:
    Primitive*  m_primitive;
    PrimitiveMarker*    m_marker;
    QPointF m_from;
    QPointF m_to;
};

#endif // MOVEMARKERCOMMAND_H
