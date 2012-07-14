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

    virtual void Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const = 0;
    virtual void Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const = 0;

    qreal sceneZoom() const;

    virtual QString name() const { return m_name; }
    virtual int childCount() const;
    virtual SceneTreeItem* child(int indx);
    virtual int indexOf(SceneTreeItem* child);
    virtual SceneTreeItem* parent() const;

    void addPrimitive(Primitive* p);
    void removePrimitive(Primitive* p);

    int primitivesCount() const;
    Primitive* primitive(int indx);

    Primitive* getPrimitiveAtPoint(const QPointF &pnt);

    void execCommand( Command* cmd );

    virtual void select( bool s);
    void selectPrimitive( Primitive* p , bool select);
signals:
    void changed();
public slots:

protected:
    QList<Primitive*>   m_primitives;
private:
    Scene*  m_scene;
    QString m_name;
};

class StaticBody : public Body {
    Q_OBJECT
public:
    explicit StaticBody(Scene* scene,const QString& name,QObject* parent=0);

    virtual void Draw( const Canvas* canvas , QPainter* painter, const QList<Primitive*>& selected ) const;
    virtual void Draw( const Canvas* canvas , QPainter* painter, const Primitive* selected ) const;

};

#endif // BODY_H
