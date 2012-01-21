#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <tr1/array>
#include <QPointF>

enum ToolType {
    ToolTypeEdit,
    ToolTypeBox,
    ToolTypesAmount
};

class Tool;
class Canvas;
class QPainter;
class Scene;
class History;

class Tools : public QObject
{
    Q_OBJECT
public:
    explicit Tools(History* history,QObject *parent = 0);
    
    ToolType toolType() const;
    void selectTool( ToolType type );

    Tool*   ActiveTool();
    const Tool* ActiveTool() const;

    bool onMousePress( const QPointF& pos );
    bool onMouseMove( const QPointF& pos );
    bool onMouseRelease( const QPointF& pos );

    void Draw(const Canvas* canvas,QPainter* painter) const;
signals:
    void changed();
public slots:
    void activateBoxTool();
    void activateEditTool();
private:
    History*    m_history;
    ToolType    m_active_tool;
    std::tr1::array<Tool*,ToolTypesAmount>  m_tools;
};

#endif // TOOLS_H
