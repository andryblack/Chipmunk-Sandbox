#ifndef CIRCLEPRIMITIVE_H
#define CIRCLEPRIMITIVE_H

#include "primitivewithstaticmarkers.h"

class CirclePrimitive : public PrimitiveWithStaticMarkers
{
    Q_OBJECT
    Q_PROPERTY(QPointF  position    READ position   WRITE setPosition   )
    Q_PROPERTY(qreal    radius    READ r   WRITE setR   )

public:
    explicit CirclePrimitive(Body *body,const QPointF& pos,qreal r,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter) const;

    qreal r() const { return m_r;}
    void setR( qreal r);

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const { return m_pos; }
    void   setPosition( const QPointF& pos );
    virtual void move( const QPointF& pos );

    virtual QString iconFile() const { return ":/icons/circle.png"; }
signals:
    
public slots:
    
private:
    void updateText();
    QPointF m_pos;
    qreal   m_r;
};

#endif // CIRCLEPRIMITIVE_H
