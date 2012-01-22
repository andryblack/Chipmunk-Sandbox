#ifndef PRIMITIVEMARKER_H
#define PRIMITIVEMARKER_H

#include <QObject>
#include <QPointF>
#include <QSizeF>

class Primitive;
class Canvas;
class QPainter;
class Command;


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

    virtual bool isPointInside( const QPointF& p) const = 0;
    virtual QPointF position() const = 0;
    virtual void move(const QPointF& pos) = 0;
    virtual bool visible() const { return true; }

    virtual PrimitiveMarkerXAlign   xAlign() const = 0;
    virtual PrimitiveMarkerYAlign   yAlign() const = 0;

    virtual void reset() {}

    const QPointF& startPoint() const { return m_start_point; }
    const QPointF& endPoint() const { return m_end_point; }

    virtual Command* generateCommand() = 0;
signals:
    
public slots:
    
protected:
    void    setStartPoint( const QPointF& pos ) {
        m_start_point = pos;
    }
    void    setEndPoint( const QPointF& pos ) {
        m_end_point = pos;
    }

private:
    QPointF     m_start_point;
    QPointF     m_end_point;
};

#endif // PRIMITIVEMARKER_H
