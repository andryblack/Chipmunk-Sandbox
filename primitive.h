#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>

#include "scenetreeitem.h"

class Canvas;
class QPainter;
class Body;
class PrimitiveMarker;

class Primitive : public SceneTreeItem
{
    Q_OBJECT
public:
    explicit Primitive(Body* scene,QObject *parent = 0);
    Body*  body();

    qreal   sceneZoom() const;

    virtual void Draw( const Canvas* canvas , QPainter* painter ) const = 0;
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const = 0;

    virtual bool isPointInside( const QPointF& p) const = 0;

    virtual QPointF position() const = 0;
    virtual void move( const QPointF& pos ) = 0;

    virtual PrimitiveMarker* getMarkerAtPoint( const QPointF& pos ) = 0;

    const QString&  text() const { return m_text; }

    QString name() const { return m_name; }

    virtual int childCount() const;
    virtual SceneTreeItem* child(int indx);
    virtual int indexOf(SceneTreeItem* child);
    virtual SceneTreeItem* parent() const;
signals:
    
public slots:
    
protected:
    void setText( const QString& t ) { m_text = t; }
    void setName( const QString& name ) { m_name = name; }
private:
    Body*  m_body;
    QString m_text;
    QString m_name;
};

#endif // PRIMITIVE_H
