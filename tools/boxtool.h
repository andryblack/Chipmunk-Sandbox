#ifndef BOXTOOL_H
#define BOXTOOL_H

#include "../tool.h"
#include <QRectF>

class BoxPrimitive;

class BoxTool : public Tool
{
    Q_OBJECT
public:
    explicit BoxTool(Scene* scene,QObject *parent = 0);
    
    virtual void Draw(const Canvas* canvas,QPainter* painter) const;

    bool onMouseMove( const QPointF& pos );
signals:
    
public slots:

protected:
    bool beginCreating(const QPointF &pos);
    void endCreating(const QPointF &pos);
private:
    BoxPrimitive*    m_primitive;
};

#endif // BOXTOOL_H
