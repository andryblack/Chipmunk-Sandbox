#ifndef POLYGONPRIMITIVE_H
#define POLYGONPRIMITIVE_H

#include "../primitive.h"
#include <QPointF>
#include <QPolygonF>

class PolygonPrimitive : public Primitive
{
    Q_OBJECT
public:
    explicit PolygonPrimitive(Scene *scene,const QPointF& pos,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter) const;

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const { return m_points.boundingRect().center(); }
    virtual void move( const QPointF& pos );
    virtual QSizeF size() const { return m_points.boundingRect().size(); }

    int pointsAmount() const { return m_points.size(); }
    QPointF point(int indx) const { return m_points[indx]; }
    const QPolygonF& points() const { return m_points; }
    qreal   connectWidth() const;
    bool    insideStart(const QPointF& pos) const;

    void    addPoint( const QPointF& pos );
    void    moveLastPoint( const QPointF& pos );
    void    removeLast();
signals:
    
public slots:
    
private:
    void updateText();
    QPolygonF    m_points;
};

#endif // POLYGONPRIMITIVE_H
