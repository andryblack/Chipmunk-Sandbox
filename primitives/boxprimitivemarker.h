#ifndef BOXPRIMITIVEMARKER_H
#define BOXPRIMITIVEMARKER_H

#include "../primitivemarker.h"

class BoxPrimitive;

class BoxPrimitiveMarker : public PrimitiveMarker
{
    Q_OBJECT
public:
    explicit BoxPrimitiveMarker(BoxPrimitive* primitive,QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter ) const;
signals:
    
public slots:
    
protected:
    BoxPrimitive*   primitive() { return m_primitive;}
    const BoxPrimitive* primitive() const { return m_primitive;}
    qreal   width() const;
    bool haveWidth() const;
    bool haveHeight() const;
private:
    BoxPrimitive*   m_primitive;
};

class BoxPrimitiveMarkerTL : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerTL(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Top : PrimitiveMarkerYAlign_Bottom; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
};

class BoxPrimitiveMarkerTR : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerTR(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Right : PrimitiveMarkerXAlign_Left; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Top : PrimitiveMarkerYAlign_Bottom; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
};

class BoxPrimitiveMarkerBL : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerBL(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Left : PrimitiveMarkerXAlign_Right; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Bottom : PrimitiveMarkerYAlign_Top; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
};

class BoxPrimitiveMarkerBR : public BoxPrimitiveMarker {
    Q_OBJECT
public:
    explicit BoxPrimitiveMarkerBR(BoxPrimitive* primitive,QObject *parent = 0) : BoxPrimitiveMarker(primitive,parent) {
    }

    virtual PrimitiveMarkerXAlign   xAlign() const { return haveWidth() ? PrimitiveMarkerXAlign_Right : PrimitiveMarkerXAlign_Left; }
    virtual PrimitiveMarkerYAlign   yAlign() const { return haveHeight() ? PrimitiveMarkerYAlign_Bottom : PrimitiveMarkerYAlign_Top; }

    virtual QSizeF size() const { return QSizeF(width(),width()); }

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
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

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
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

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
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

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
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

    virtual QPointF position() const;
    virtual void move(const QPointF& pos);
};


#endif // BOXPRIMITIVEMARKER_H
