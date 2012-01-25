#ifndef MOVEPOLYGONMARKERCOMMAND_H
#define MOVEPOLYGONMARKERCOMMAND_H

#include <QPointF>
#include "../command.h"

class PolygonPrimitive;

class MovePolygonMarkerCommand : public Command
{
    Q_OBJECT
public:
    explicit MovePolygonMarkerCommand(PolygonPrimitive* p,
                                      int index,
                                      const QPointF& from,
                                      const QPointF& to,QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;

signals:
    
public slots:
    
private:
    PolygonPrimitive*    m_primitive;
    int m_index;
    QPointF m_from;
    QPointF m_to;
};

#endif // MOVEPOLYGONMARKERCOMMAND_H
