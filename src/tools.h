#ifndef TOOLS_H
#define TOOLS_H

#include <QObject>
#include <QPointF>

enum ToolType {
    ToolTypeEdit,
    ToolTypeBox,
    ToolTypeCircle,
    ToolTypePolygon,
    ToolTypesAmount
};

class Tool;
class Canvas;
class QPainter;
class Scene;

class Tools : public QObject
{
    Q_OBJECT
public:
    explicit Tools(Scene* scene,QObject *parent = 0);
    
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
    void activateCircleTool();
    void activatePolygonTool();
    void activateEditTool();
private:
    Scene*    m_scene;
    ToolType    m_active_tool;
    Tool*  m_tools[ToolTypesAmount];
};

#endif // TOOLS_H
