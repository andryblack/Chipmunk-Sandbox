#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QString>

#include <QAbstractItemModel>

class Primitive;
class Body;
class Canvas;
class QPainter;
class Command;
class History;

class Scene : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Scene(History* history,QObject *parent = 0);

    void makeNew(const QSize &size);

    bool load(const QString& fn);
    void save();
    void save(const QString& fn);
    
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

    const QSize&  worldSize() const { return m_worldSize; }

    qreal   zoom() const { return m_zoom;}
    void    setZoom( qreal z );
    bool zoomInAllowed() const;
    bool zoomOutAllowed() const;
    bool zoomActualAllowed() const;

    const  QString& text() const { return m_text;}
    void   setText( const QString& text );

    void execCommand( Command* cmd );

    int bodysCount() const;
    Body* body(int indx) const;

    //Body* staticBody();
    int bodyIndex( Body* b) const;

    void addBody( Body* b );
    void removeBody( Body* b );

    const QString& fileName() const { return m_filename;}



    //// tree model
    QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;

signals:
    void zoomChanged();
    void textChanged();

    void changed();
    void selectionChanged();
    void selectionReset();

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

    QSize       m_worldSize;
    qreal       m_zoom;

    QString m_text;

    QList<Body*>    m_bodys;
    Body*   m_static_body;

    QString m_filename;

    QModelIndex getBodyIndex(Body* body);
};

#endif // SCENE_H
