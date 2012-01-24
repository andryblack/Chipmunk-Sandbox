#ifndef CIRCLEPRIMITIVE_H
#define CIRCLEPRIMITIVE_H

#include "primitivewithstaticmarkers.h"

class CirclePrimitive : public PrimitiveWithStaticMarkers
{
    Q_OBJECT
public:
    explicit CirclePrimitive(Scene *scene,const QPointF& pos,qreal r,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter) const;

    qreal r() const { return m_r;}
    void setR( qreal r);

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const { return m_pos; }
    virtual void move( const QPointF& pos );

signals:
    
public slots:
    
private:
    void updateText();
    QPointF m_pos;
    qreal   m_r;
};

#endif // CIRCLEPRIMITIVE_H
