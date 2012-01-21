#ifndef PRIMITIVEMARKER_H
#define PRIMITIVEMARKER_H

#include <QObject>
#include <QPointF>
#include <QSizeF>

class Primitive;
class Canvas;
class QPainter;

enum PrimitiveMarkerXAlign {
    PrimitiveMarkerXAlign_Left,
    PrimitiveMarkerXAlign_Center,
    PrimitiveMarkerXAlign_Right
};

enum PrimitiveMarkerYAlign {
    PrimitiveMarkerYAlign_Top,
    PrimitiveMarkerYAlign_Center,
    PrimitiveMarkerYAlign_Bottom
};


class PrimitiveMarker : public QObject
{
    Q_OBJECT
public:
    explicit PrimitiveMarker(QObject *parent = 0);
    
    virtual void Draw( const Canvas* canvas , QPainter* painter ) const = 0;

    bool isPointInside( const QPointF& p) const;
    virtual QPointF position() const = 0;
    virtual void move(const QPointF& pos) = 0;
    virtual bool visible() const { return true; }

    virtual PrimitiveMarkerXAlign   xAlign() const = 0;
    virtual PrimitiveMarkerYAlign   yAlign() const = 0;

    virtual QSizeF size() const = 0;
signals:
    
public slots:
    
protected:
private:
};

#endif // PRIMITIVEMARKER_H
