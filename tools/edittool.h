#ifndef EDITTOOL_H
#define EDITTOOL_H

#include "../tool.h"

class Primitive;
class PrimitiveMarker;

class EditTool : public Tool
{
    Q_OBJECT
public:
    explicit EditTool(History* history,QObject *parent = 0);
    
    virtual void Activate();
    virtual bool onMousePress( const QPointF& pos );
    virtual bool onMouseMove( const QPointF &pos );
    virtual bool onMouseRelease(const QPointF &pos);
signals:
    
public slots:
    
private:
    Primitive*  m_selected;
    QPointF m_offset;
    QPointF m_start_pos;
    bool    m_moved;
    PrimitiveMarker*    m_selected_marker;
};

#endif // EDITTOOL_H
