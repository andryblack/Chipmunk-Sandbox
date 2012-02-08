#ifndef POLYGONPRIMITIVE_H
#define POLYGONPRIMITIVE_H

#include "../primitive.h"
#include <QPointF>
#include <QPolygonF>

class PolygonPrimitiveMarker;

class PolygonPrimitive : public Primitive
{
    Q_OBJECT
public:
    explicit PolygonPrimitive(Body *body,const QPointF& pos,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter) const;
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const;

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const { return m_points.boundingRect().center(); }
    virtual void move( const QPointF& pos );
    virtual QSizeF size() const { return m_points.boundingRect().size(); }

    int pointsAmount() const { return m_points.size(); }
    QPointF point(int indx) const { return m_points[indx]; }
    const QPolygonF& points() const { return m_points; }
    qreal   connectWidth() const;
    bool    insideStart(const QPointF& pos) const;

    virtual PrimitiveMarker* getMarkerAtPoint( const QPointF& pos );

    PolygonPrimitiveMarker* getCornerMarkerAtPoint( const QPointF& pos,int andNot  );
    PolygonPrimitiveMarker* getNearCornerMarkerAtPoint( const QPointF& pos,int index  );

    void    addPoint( const QPointF& pos );
    void    moveLastPoint( const QPointF& pos );
    void    movePoint( int index, const QPointF& pos );
    void    removeLast();

    void    removePoint( int index );
    void    addPoint( int index, const QPointF& pos);

    virtual QString iconFile() const { return ":/icons/polygon.png"; }
signals:
    
public slots:
    
private:
    void updateText();
    QPolygonF    m_points;
    QVector<PolygonPrimitiveMarker*>    m_corner_markers;
};

#endif // POLYGONPRIMITIVE_H
