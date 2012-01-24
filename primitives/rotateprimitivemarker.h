#ifndef ROTATEPRIMITIVEMARKER_H
#define ROTATEPRIMITIVEMARKER_H

#include "../primitivemarker.h"

class Primitive;

class RotatePrimitiveMarker : public PrimitiveMarker
{
    Q_OBJECT
public:
    explicit RotatePrimitiveMarker(Primitive* primitive,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter ) const;

    qreal radius() const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return PrimitiveMarkerXAlign_Center; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return PrimitiveMarkerYAlign_Center; }

    virtual QSizeF size() const { return QSizeF(radius(),radius()); }

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);

    virtual bool isPointInside( const QPointF& p) const;

    virtual void reset();

    virtual Command* generateCommand();


    virtual void rotatePrimitive( qreal a ) = 0;
protected:

    virtual qreal   primitiveAngle() const = 0;

    qreal   beginAngle() const;
    qreal   endAngle() const;

signals:
    
public slots:
    
private:
    Primitive*  m_primitive;

    qreal       m_beginAngle;
    qreal       m_endAngle;

    qreal       m_delta_angle;

    bool        m_rotate_activated;
};

#endif // ROTATEPRIMITIVEMARKER_H
