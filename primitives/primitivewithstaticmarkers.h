#ifndef PRIMITIVEWITHSTATICMARKERS_H
#define PRIMITIVEWITHSTATICMARKERS_H

#include "../primitive.h"

class PrimitiveWithStaticMarkers : public Primitive
{
    Q_OBJECT
public:
    explicit PrimitiveWithStaticMarkers(Scene* scene,QObject *parent = 0);
    
    virtual void DrawMarkers( const Canvas* canvas, QPainter* painter ) const;

    virtual PrimitiveMarker* getMarkerAtPoint( const QPointF& pos );

signals:
    
public slots:
    
protected:
    void addMarker( PrimitiveMarker* marker );
private:

    QVector<PrimitiveMarker*>   m_markers;

};

#endif // PRIMITIVEWITHSTATICMARKERS_H
