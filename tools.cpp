#include "tools.h"

#include "tools/edittool.h"
#include "tools/boxtool.h"

#include "history.h"

Tools::Tools(History* history,QObject *parent) :
    QObject(parent), m_history(history)
{
    m_tools[ToolTypeEdit] = new EditTool(history,this);
    m_tools[ToolTypeBox] = new BoxTool(history,this);

    m_active_tool = ToolTypeBox;
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
    if (ActiveTool()->creating())
        ActiveTool()->Draw(canvas,painter);
}

void Tools::activateBoxTool() {
     selectTool(ToolTypeBox);
}

void Tools::activateEditTool() {
    selectTool(ToolTypeEdit);
}

void Tools::selectTool( ToolType type ) {
    if (m_active_tool!=type) {
        m_active_tool = type;
        m_history->setText("");
        ActiveTool()->Activate();
        emit changed();
    }
}
