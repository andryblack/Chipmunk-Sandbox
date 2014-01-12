#ifndef MOVEBODYCOMMAND_H
#define MOVEBODYCOMMAND_H

#include "../command.h"
#include <QPointF>

class Body;

class MoveBodyCommand : public Command
{
    Q_OBJECT
public:
    explicit MoveBodyCommand(Body* body,const QPointF& from,QPointF& to,QObject *parent = 0);

    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
signals:

public slots:
private:
    Body*   m_body;
    QPointF m_from;
    QPointF m_to;
};

#endif // MOVEBODYCOMMAND_H
