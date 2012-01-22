#ifndef BOXPRIMITIVEMARKER_H
#define BOXPRIMITIVEMARKER_H

#include "../primitivemarker.h"
#include "rotateprimitivemarker.h"

class BoxPrimitive;

class BoxPrimitiveMarker : public PrimitiveMarker
{
    Q_OBJECT
public:
    explicit BoxPrimitiveMarker(BoxPrimitive* primitive,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter ) const;
    virtual QPointF position( ) const;

    virtual bool isPointInside( const QPointF& p) const;
    const BoxPrimitive* primitive() const { return m_primitive;}

    QPointF untransformPoint( const QPointF& pos ) const;
    QPointF transformPoint( const QPointF& pos ) const;
signals:
    
public slots:
    
protected:
    BoxPrimitive*   primitive() { return m_primitive;}
    virtual bool move_impl( const QPointF& pos) = 0;
    virtual QPointF position_impl( ) const = 0;


    qreal   width() const;
    bool haveWidth() const;
    bool haveHeight() const;

    virtual void move(const QPointF& pos);
    virtual void reset();

    virtual Command* generateCommand() ;
private:
    BoxPrimitive*   m_primitive;
    bool    m_activated;
};

class BoxPrimitiveMarkerTL : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerTL(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Top : PrimitiveMarkerYAlign_Bottom; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerTR : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerTR(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Right : PrimitiveMarkerXAlign_Left; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Top : PrimitiveMarkerYAlign_Bottom; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerBL : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerBL(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Bottom : PrimitiveMarkerYAlign_Top; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerBR : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerBR(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Right : PrimitiveMarkerXAlign_Left; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Bottom : PrimitiveMarkerYAlign_Top; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerR : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerR(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual bool visible() const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Right : PrimitiveMarkerXAlign_Left; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return PrimitiveMarkerYAlign_Center; }

    virtual QSizeF size() const;

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerL : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerL(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual bool visible() const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return PrimitiveMarkerYAlign_Center; }

    virtual QSizeF size() const;

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerT : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerT(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual bool visible() const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return PrimitiveMarkerXAlign_Center; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Top : PrimitiveMarkerYAlign_Bottom; }

    virtual QSizeF size() const;

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class BoxPrimitiveMarkerB : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerB(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual bool visible() const;

    virtual PrimitiveMarkerXAlign   xAlign() const { return PrimitiveMarkerXAlign_Center; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Bottom : PrimitiveMarkerYAlign_Top; }

    virtual QSizeF size() const;

    virtual QPointF position_impl() const;
    virtual bool move_impl(const QPointF& pos);
};

class RotateBoxPrimitiveMarker : public RotatePrimitiveMarker {
    Q_OBJECT
public:
    RotateBoxPrimitiveMarker( BoxPrimitive* primitive, QObject* parent = 0);

protected:
    virtual qreal   primitiveAngle() const ;
    virtual void rotatePrimitive( qreal a ) ;
private:
    BoxPrimitive*   m_box;
};


#endif // BOXPRIMITIVEMARKER_H
