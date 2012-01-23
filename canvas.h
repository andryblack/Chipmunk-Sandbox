#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

class BoxPrimitive;
class CirclePrimitive;
class PolygonPrimitive;
class QPainter;
class Tools;
class Scene;
class BoxPrimitiveMarker;
class RotatePrimitiveMarker;
class CirclePrimitiveMarker;

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(Tools* tools,Scene* scene,QWidget *parent = 0);
    
    bool drawGrid() const;
    bool snapToGrid() const;


    void Draw(const BoxPrimitive* box, QPainter* painter) const;
    void Draw(const BoxPrimitiveMarker* marker, QPainter* painter) const;

    void Draw(const RotatePrimitiveMarker* marker, QPainter* painter) const;

    void Draw(const CirclePrimitive* circle, QPainter* painter) const;
    void Draw(const CirclePrimitiveMarker* marker, QPainter* painter) const;

    void Draw(const PolygonPrimitive* polygon, QPainter* painter,bool creating=false) const;


public slots:
    void setDrawGrid(bool draw);
    void setSnapToGrid(bool snap);
    void onZoomChanged();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);


    QSize   size() const;
    qreal   zoom() const;

private:
    Tools*  m_tools;
    Scene*  m_scene;
    bool    m_drawGrid;
    int     m_gridSize;
    bool    m_snapToGrid;
    void processPos(QPointF& pos) const;
};

#endif // CANVAS_H
