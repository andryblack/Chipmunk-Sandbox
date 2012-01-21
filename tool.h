#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPointF>

class Canvas;
class QPainter;
class History;

class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(History* history,QObject *parent = 0);
    
    virtual void Activate();

    virtual void Draw(const Canvas* canvas,QPainter* painter) const;

    bool creating() const;

    virtual bool onMousePress( const QPointF& pos );
    virtual bool onMouseMove( const QPointF& pos );
    virtual bool onMouseRelease( const QPointF& pos );

signals:
    
public slots:

protected:
    History* history() { return m_history;}
    virtual bool beginCreating( const QPointF& pos );
    const QPointF& beginPos() const { return m_begin_pos;}
    virtual void endCreating( const QPointF& pos );
private:
    History*    m_history;
    bool    m_creating;
    QPointF m_begin_pos;
};

#endif // TOOL_H
