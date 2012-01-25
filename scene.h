#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QString>

class Primitive;
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
    void DrawSelected( const Canvas* canvas, QPainter* painter) const;
    void DrawMarkers( const Canvas* canvas, QPainter* painter) const;

    void appendPrimitive(Primitive* p);
    void removePrimitive(Primitive* p);

    Primitive* getPrimitiveAtPoint(const QPointF& p);

    void clearSelection();
    void setSelected(Primitive* p);
    void addSelected(Primitive* p);
    Primitive* selected() const;

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

signals:
    void zoomChanged();
    void textChanged();

    void changed();

public slots:
    void incrementZoom();
    void decrementZoom();
    void actualZoom();

    void undo();
    void redo();

private:
    History*    m_history;
    QList<Primitive*> m_free_primtives;
    QList<Primitive*> m_selected;

    QSizeF       m_worldSize;
    qreal       m_zoom;

    QString m_text;
};

#endif // SCENE_H
