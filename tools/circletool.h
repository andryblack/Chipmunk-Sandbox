#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include "../tool.h"

class CirclePrimitive;

class CircleTool : public Tool
{
    Q_OBJECT
public:
    explicit CircleTool(Scene* scene,QObject *parent = 0);
    
    virtual void Draw(const Canvas* canvas,QPainter* painter) const;

    bool onMouseMove( const QPointF& pos );
signals:
    
public slots:
    
protected:
    bool beginCreating(const QPointF &pos);
    void endCreating(const QPointF &pos);
private:
    CirclePrimitive*    m_primitive;
};

#endif // CIRCLETOOL_H
