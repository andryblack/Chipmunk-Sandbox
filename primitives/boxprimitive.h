#ifndef BOXPRIMITIVE_H
#define BOXPRIMITIVE_H

#include "primitivewithstaticmarkers.h"

#include <QRectF>

class BoxPrimitive : public PrimitiveWithStaticMarkers
{
    Q_OBJECT
public:
    explicit BoxPrimitive(Body *body,const QRectF &rect , QObject *parent= 0);
    
    const QRectF& rect() const { return m_rect; }

    virtual void Draw( const Canvas* canvas , QPainter* painter) const;

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const;
    virtual QSizeF size() const;

    virtual void move( const QPointF& pos );

    void setSize( const QSizeF& size );

    bool    setTopLeft( const QPointF& pos );
    bool    setTopRight( const QPointF& pos );
    bool    setBottomRight( const QPointF& pos );
    bool    setBottomLeft( const QPointF& pos );
    bool    setRight( qreal r );
    bool    setLeft( qreal l );
    bool    setTop( qreal t );
    bool    setBottom( qreal b );

    qreal   angle() const { return m_angle;}
    void    rotate( qreal a );

    QPointF untransformPoint( const QPointF& pos ) const;
    QPointF transformPoint( const QPointF& pos ) const;

    virtual QString iconFile() const { return ":/icons/box.png"; }
protected:
signals:
    
public slots:
    
private:
    QRectF  m_rect;
    void updateText();
    qreal   m_angle;
};

#endif // BOXPRIMITIVE_H
