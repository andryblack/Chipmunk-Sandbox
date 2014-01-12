#ifndef BODYTOOL_H
#define BODYTOOL_H

#include "../tool.h"

class Body;

class BodyTool : public Tool
{
    Q_OBJECT
public:
    explicit BodyTool(Scene* scene,QObject *parent = 0);

    virtual void Activate();
    virtual void Draw(const Canvas* canvas,QPainter* painter) const;

    virtual bool onMousePress( const QPointF& pos );
    virtual bool onMouseMove( const QPointF &pos );
    virtual bool onMouseRelease(const QPointF &pos);
signals:

public slots:

private:
    Body*   m_moved_body;
    QPointF m_start_pos;
    QPointF m_offset;
};

#endif // BODYTOOL_H
