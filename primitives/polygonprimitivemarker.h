#ifndef POLYGONPRIMITIVEMARKER_H
#define POLYGONPRIMITIVEMARKER_H

#include "../primitivemarker.h"

class PolygonPrimitive;

class PolygonPrimitiveMarker : public PrimitiveMarker
{
    Q_OBJECT
public:
    explicit PolygonPrimitiveMarker(PolygonPrimitive* primitive,int index,QObject *parent = 0);
    
    void Draw( const Canvas* canvas , QPainter* painter ) const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return PrimitiveMarkerXAlign_Center; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return PrimitiveMarkerYAlign_Center; }

    virtual bool isPointInside( const QPointF& pos) const;
    virtual QPointF position() const;
    virtual void move(const QPointF& pos);

    virtual qreal width() const;

    virtual void complete(Scene* scene);
signals:
    
public slots:
    
private:
    PolygonPrimitive*   m_primitive;
    int     m_index;
};

class PolygonPrimitiveSubdivMarkerGhost : public PolygonPrimitiveMarker
{
    Q_OBJECT
public:
    PolygonPrimitiveSubdivMarkerGhost( const QPointF& pos, qreal w ) :
        PolygonPrimitiveMarker( 0,0,0 ),m_pos(pos),m_width(w) {

    }
    virtual qreal width() const { return m_width;}
    virtual QPointF position() const { return m_pos; }
private:
    QPointF m_pos;
    qreal m_width;
};

#endif // POLYGONPRIMITIVEMARKER_H
