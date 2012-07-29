#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>
#include <QFlag>

#include "scenetreeitem.h"

class Canvas;
class QPainter;
class Body;
class PrimitiveMarker;

class Primitive : public SceneTreeItem
{
    Q_OBJECT
    Q_FLAGS(CollisionTypeBit     CollisionTypeBits)
    Q_PROPERTY(qreal  friction    READ friction   WRITE setFriction   )
    Q_PROPERTY(qreal  elasticity    READ elasticity   WRITE setElasticity  )
    Q_PROPERTY(bool   sensor    READ sensor   WRITE setSensor  )
    Q_PROPERTY(CollisionTypeBits   collisionType    READ collisionType   WRITE setCollisionType  )

public:
    enum CollisionTypeBit {
        Bit0 = 1 << 0,
        Bit1 = 1 << 1,
        Bit2 = 1 << 2,
        Bit3 = 1 << 3,
        Bit4 = 1 << 4,
        Bit5 = 1 << 5,
        Bit6 = 1 << 6,
        Bit7 = 1 << 7,
        Bit8 = 1 << 8,
        Bit9 = 1 << 9,
        Bit10 = 1 << 10,
        Bit11 = 1 << 11,
        Bit12 = 1 << 12,
        Bit13 = 1 << 13,
        Bit14 = 1 << 14,
        Bit15 = 1 << 15
    };
    Q_DECLARE_FLAGS(CollisionTypeBits, CollisionTypeBit)

    explicit Primitive(Body* body,QObject *parent = 0);
    Body*  body();

    qreal   sceneZoom() const;

    virtual void Draw( const Canvas* canvas , QPainter* painter ) const = 0;
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const = 0;

    virtual bool isPointInside( const QPointF& p) const = 0;

    virtual QPointF position() const = 0;
    virtual void move( const QPointF& pos ) = 0;

    virtual PrimitiveMarker* getMarkerAtPoint( const QPointF& pos ) = 0;

    const QString&  text() const { return m_text; }

    void setName( const QString& name ) {
        setObjectName(name);
        emit propertyChanged();
    }
    QString name() const { return objectName(); }

    virtual int childCount() const;
    virtual SceneTreeItem* child(int indx);
    virtual int indexOf(SceneTreeItem* child);
    virtual SceneTreeItem* parent() const;

    virtual void select( bool s );
    virtual bool selected() const;

    virtual void update() {}

    qreal friction() const { return m_friction; }
    void setFriction(qreal f) { m_friction = f; emit propertyChanged(); }
    qreal elasticity() const { return m_elasticity; }
    void setElasticity(qreal e) { m_elasticity = e; emit propertyChanged(); }
    bool sensor() const { return m_sensor; }
    void setSensor(bool s) { m_sensor = s; emit propertyChanged(); }
    CollisionTypeBits collisionType() const { return m_collisionType; }
    void setCollisionType(CollisionTypeBits ct) { m_collisionType = ct; emit propertyChanged(); }
signals:
    
public slots:
    
protected:
    void setText( const QString& t ) { m_text = t; }
private:
    Body*  m_body;
    QString m_text;
    qreal   m_friction;
    qreal   m_elasticity;
    bool    m_sensor;
    CollisionTypeBits   m_collisionType;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Primitive::CollisionTypeBits)

#endif // PRIMITIVE_H
