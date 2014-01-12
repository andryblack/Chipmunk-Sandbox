#include "tools.h"

#include "tools/edittool.h"
#include "tools/boxtool.h"
#include "tools/circletool.h"
#include "tools/polygontool.h"
#include "tools/bodytool.h"

#include "scene.h"

Tools::Tools(Scene *scene,QObject *parent) :
    QObject(parent), m_scene(scene)
{
    m_tools[ToolTypeEdit] = new EditTool(scene,this);
    m_tools[ToolTypeBox] = new BoxTool(scene,this);
    m_tools[ToolTypeCircle] = new CircleTool(scene,this);
    m_tools[ToolTypePolygon] = new PolygonTool(scene,this);
    m_tools[ToolTypeBody] = new BodyTool(scene,this);

    m_active_tool = ToolTypeEdit;
}

ToolType Tools::toolType() const {
    return m_active_tool;
}

Tool* Tools::ActiveTool() {
    return m_tools[m_active_tool];
}
const Tool* Tools::ActiveTool() const {
    return m_tools[m_active_tool];
}

bool Tools::onMousePress( const QPointF& pos ) {
    return ActiveTool()->onMousePress(pos);
}

bool Tools::onMouseMove( const QPointF& pos ) {
    return ActiveTool()->onMouseMove(pos);
}

bool Tools::onMouseRelease( const QPointF& pos ) {
    return ActiveTool()->onMouseRelease(pos);
}

void Tools::Draw(const Canvas* canvas,QPainter* painter) const {
    ActiveTool()->Draw(canvas,painter);
}

void Tools::activateBoxTool() {
     selectTool(ToolTypeBox);
}

void Tools::activateCircleTool() {
     selectTool(ToolTypeCircle);
}

void Tools::activatePolygonTool() {
     selectTool(ToolTypePolygon);
}

void Tools::activateEditTool() {
    selectTool(ToolTypeEdit);
}

void Tools::activateBodyTool()
{
    selectTool(ToolTypeBody);
}

void Tools::selectTool( ToolType type ) {
    if (m_active_tool!=type) {
        m_active_tool = type;
        m_scene->setText("");
        ActiveTool()->Activate();
        emit changed();
    }
}
