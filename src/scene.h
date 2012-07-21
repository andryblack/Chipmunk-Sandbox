#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QString>

class Primitive;
class Body;
class Canvas;
class QPainter;
class Command;
class History;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(History* history,QObject *parent = 0);
    
    void Draw( const Canvas* canvas, QPainter* painter) const;

    Primitive* getPrimitiveAtPoint(const QPointF& p);

    void clearSelection();
    void setSelected(Primitive* p);
    void addSelected(Primitive* p);
    void removeSelected(Primitive* p);
    Primitive* selected() const;
    bool selected(const Primitive *p) const;
    const QList<Primitive*>& selectedPrimitives() const { return m_selected; }

    void setSelected(Body* b);
    void addSelected(Body* p);
    void removeSelected(Body* p);
    const QList<Body*>& selectedBodies() const { return m_body_selected; }
    void setActiveBody(Body* b);
    Body*   activeBody() const { return m_active_body ? m_active_body : m_static_body; }

    void update();

    const QSizeF&  worldSize() const { return m_worldSize; }

    qreal   zoom() const { return m_zoom;}
    void    setZoom( qreal z );
    bool zoomInAllowed() const;
    bool zoomOutAllowed() const;
    bool zoomActualAllowed() const;

    const  QString& text() const { return m_text;}
    void   setText( const QString& text );

    void execCommand( Command* cmd );

    int bodysCount() const;
    Body* body(int indx);

    //Body* staticBody();
    int bodyIndex( Body* b) const;

    void addBody( Body* b );
    void removeBody( Body* b );
signals:
    void zoomChanged();
    void textChanged();

    void changed();
    void selectionChanged();

public slots:
    void incrementZoom();
    void decrementZoom();
    void actualZoom();

    void undo();
    void redo();

protected slots:
    void bodyChanged();
private:
    History*    m_history;

    QList<Primitive*> m_selected;
    QList<Body*>    m_body_selected;
    Body*   m_active_body;

    QSizeF       m_worldSize;
    qreal       m_zoom;

    QString m_text;

    QList<Body*>    m_bodys;
    Body*   m_static_body;
};

#endif // SCENE_H
