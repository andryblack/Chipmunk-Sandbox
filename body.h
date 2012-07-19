#ifndef BODY_H
#define BODY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPointF>

#include "scenetreeitem.h"

class Primitive;
class Scene;
class Command;
class QPainter;
class Canvas;


class Body : public SceneTreeItem
{
    Q_OBJECT
public:
    explicit Body(Scene* scene,const QString& name,QObject *parent = 0);

    virtual void Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const;
    virtual void Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const;

    qreal sceneZoom() const;

    virtual QString name() const { return objectName(); }
    virtual int childCount() const;
    virtual SceneTreeItem* child(int indx);
    virtual int indexOf(SceneTreeItem* child);
    virtual SceneTreeItem* parent() const;

    void addPrimitive(Primitive* p);
    void removePrimitive(Primitive* p);

    int primitivesCount() const;
    Primitive* primitive(int indx);
    int primitiveIndex(Primitive *p) const;

    Primitive* getPrimitiveAtPoint(const QPointF &pnt);

    void execCommand( Command* cmd );

    virtual void select( bool s);
    void selectPrimitive( Primitive* p , bool select);

    virtual bool active() const;
signals:
    void changed();
public slots:

protected:
    QList<Primitive*>   m_primitives;
private:
    Scene*  m_scene;
};

class StaticBody : public Body {
    Q_OBJECT
public:
    explicit StaticBody(Scene* scene,const QString& name,QObject* parent=0);

    virtual QString iconFile() const;
};

class DynamicBody : public Body {
    Q_OBJECT
    Q_PROPERTY(qreal   mass        READ mass       WRITE setMass   )
    Q_PROPERTY(qreal   moment      READ moment     WRITE setMoment   )
    Q_PROPERTY(QPointF  position    READ position   WRITE setPosition   )
    Q_PROPERTY(QPointF  velocity    READ velocity   WRITE setVelocity )
    Q_PROPERTY(QPointF  force       READ force      WRITE setForce )
    Q_PROPERTY(qreal   angle       READ angle      WRITE setAngle   )
    Q_PROPERTY(qreal   angleVelocity       READ angleVelocity      WRITE setAngleVelocity  )
    Q_PROPERTY(qreal   torque       READ torque      WRITE setTorque   )
    Q_PROPERTY(qreal   velocityLimit       READ velocityLimit      WRITE setVelocityLimit   )
    Q_PROPERTY(qreal   angleVelocityLimit       READ angleVelocityLimit      WRITE setAngleVelocityLimit   )
public:
    explicit DynamicBody(Scene* scene,const QString& name,QObject* parent=0);

    virtual void Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const;
    virtual void Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const;

    qreal mass() const { return m_mass; }
    void setMass(qreal m) { m_mass = m; emit propertyChanged(); }

    qreal moment() const { return m_moment; }
    void setMoment(qreal m) { m_moment = m; emit propertyChanged(); }

    QPointF position() const { return m_position; }
    void setPosition(const QPointF& v) { m_position = v; emit propertyChanged(); }

    QPointF velocity() const { return m_velocity; }
    void setVelocity(const QPointF& v) { m_velocity = v; emit propertyChanged(); }

    QPointF force() const { return m_force; }
    void setForce(const QPointF& v) { m_force = v; emit propertyChanged(); }

    qreal angle() const { return m_angle; }
    void setAngle(qreal m) { m_angle = m; emit propertyChanged(); }

    qreal angleVelocity() const { return m_angle_vel; }
    void setAngleVelocity(qreal m) { m_angle_vel = m; emit propertyChanged(); }

    qreal torque() const { return m_torque; }
    void setTorque(qreal m) { m_torque = m; emit propertyChanged(); }

    qreal velocityLimit() const { return m_vel_limit; }
    void setVelocityLimit(qreal m) { m_vel_limit = m; emit propertyChanged(); }

    qreal angleVelocityLimit() const { return m_angle_vel_limit; }
    void setAngleVelocityLimit(qreal m) { m_angle_vel_limit = m; emit propertyChanged(); }

    virtual QString iconFile() const;
private:
    qreal  m_mass;
    qreal  m_moment;
    QPointF m_position;
    QPointF m_velocity;
    QPointF m_force;
    qreal  m_angle;
    qreal  m_angle_vel;
    qreal  m_torque;
    qreal  m_vel_limit;
    qreal  m_angle_vel_limit;
};

#endif // BODY_H
