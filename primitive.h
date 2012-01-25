#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QObject>
#include <QPointF>
#include <QVector>
#include <QString>

class Canvas;
class QPainter;
class Scene;
class PrimitiveMarker;

class Primitive : public QObject
{
    Q_OBJECT
public:
    explicit Primitive(Scene* scene,QObject *parent = 0);

    Scene*  scene();
    qreal   sceneZoom() const;

    virtual void Draw( const Canvas* canvas , QPainter* painter ) const = 0;
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const = 0;

    virtual bool isPointInside( const QPointF& p) const = 0;

    virtual QPointF position() const = 0;
    virtual void move( const QPointF& pos ) = 0;

    virtual PrimitiveMarker* getMarkerAtPoint( const QPointF& pos ) = 0;

    const QString&  text() const { return m_text; }

    const QString& name() const { return m_name; }
signals:
    
public slots:
    
protected:
    void setText( const QString& t ) { m_text = t; }
    void setName( const QString& name ) { m_name = name; }
private:
    Scene*  m_scene;
    QString m_text;
    QString m_name;
};

#endif // PRIMITIVE_H
