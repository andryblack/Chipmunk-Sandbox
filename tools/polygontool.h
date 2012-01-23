#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include "../tool.h"

class PolygonPrimitive;

class PolygonTool : public Tool
{
    Q_OBJECT
public:
    explicit PolygonTool(History* history,QObject *parent = 0);
    
    virtual void Draw(const Canvas* canvas,QPainter* painter) const;

    virtual void Activate();

    virtual bool onMousePress( const QPointF& pos );
    virtual bool onMouseMove( const QPointF& pos );
    virtual bool onMouseRelease( const QPointF& pos );
signals:
    
public slots:

protected:
    bool beginCreating(const QPointF &pos);
    void endCreating(const QPointF &pos);
private:
    PolygonPrimitive*   m_primitive;
};

#endif // POLYGONTOOL_H
