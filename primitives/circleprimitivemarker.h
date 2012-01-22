#ifndef CIRCLEPRIMITIVEMARKER_H
#define CIRCLEPRIMITIVEMARKER_H

#include "../primitivemarker.h"

class CirclePrimitive;

class CirclePrimitiveMarker : public PrimitiveMarker
{
    Q_OBJECT
public:
    explicit CirclePrimitiveMarker(CirclePrimitive* primitive,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter ) const;
    virtual QPointF position( ) const;

    virtual bool isPointInside( const QPointF& p) const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return PrimitiveMarkerYAlign_Center; }

    qreal   width() const;

    const CirclePrimitive* primitive() const { return m_primitive;}

    void    move( const QPointF& pos );

    Command*    generateCommand();
signals:
    
public slots:

protected:
    bool haveWidth() const;
private:
    CirclePrimitive*    m_primitive;
};

#endif // CIRCLEPRIMITIVEMARKER_H
