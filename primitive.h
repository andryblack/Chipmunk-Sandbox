#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>

#include "primitivemarker.h"

class Canvas;
class QPainter;
class Scene;

class Primitive : public QObject
{
    Q_OBJECT
public:
    explicit Primitive(Scene* scene,QObject *parent = 0);

    qreal   sceneZoom() const;

    virtual void Draw( const Canvas* canvas , QPainter* painter ) const = 0;
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const;

    virtual bool isPointInside( const QPointF& p) const = 0;

    virtual QPointF position() const = 0;
    virtual void move( const QPointF& pos ) = 0;

    PrimitiveMarker* getMarkerAtPoint( const QPointF& pos );

    const QString&  text() const { return m_text; }
signals:
    
public slots:
    
protected:
    void addMarker( PrimitiveMarker* marker );
    void setText( const QString& t ) { m_text = t; }
private:
    Scene*  m_scene;
    QVector<PrimitiveMarker*>   m_markers;
    QString m_text;
};

#endif // PRIMITIVE_H
