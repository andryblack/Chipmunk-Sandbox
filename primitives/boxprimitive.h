#ifndef BOXPRIMITIVE_H
#define BOXPRIMITIVE_H

#include "../primitive.h"

#include <QRectF>

class BoxPrimitive : public Primitive
{
    Q_OBJECT
public:
    explicit BoxPrimitive(Scene *scene,const QRectF &rect , QObject *parent= 0);
    
    const QRectF& rect() const { return m_rect; }

    virtual void Draw( const Canvas* canvas , QPainter* painter) const;

    virtual bool isPointInside( const QPointF& p) const;
    virtual QPointF position() const;
    virtual void move( const QPointF& pos );

    void setSize( const QSizeF& size );

    void    setTopLeft( const QPointF& pos );
    void    setTopRight( const QPointF& pos );
    void    setBottomRight( const QPointF& pos );
    void    setBottomLeft( const QPointF& pos );
    void    setRight( qreal r );
    void    setLeft( qreal l );
    void    setTop( qreal t );
    void    setBottom( qreal b );
signals:
    
public slots:
    
private:
    QRectF  m_rect;
    void updateText();
};

#endif // BOXPRIMITIVE_H
