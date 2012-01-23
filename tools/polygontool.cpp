#include "polygontool.h"
#include "../history.h"
#include "../primitives/polygonprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../canvas.h"

PolygonTool::PolygonTool(History *history, QObject *parent) :
    Tool(history,parent),m_primitive(0)
{
}

void PolygonTool::Activate() {
    if (m_primitive) {
        delete m_primitive;
        m_primitive = 0;
    }
}

void PolygonTool::Draw(const Canvas* canvas,QPainter* painter) const {
    if (m_primitive)
        canvas->Draw( m_primitive, painter, true );
}

bool PolygonTool::onMousePress( const QPointF& pos ) {
    if (!m_primitive) {
        if (Tool::onMousePress(pos)) {
            m_primitive->addPoint(pos);
            return true;
        }
        return false;
    }

    {
        if (m_primitive->insideStart(pos)) {
            endCreating(pos);
        } else {
            m_primitive->addPoint(pos);
        }
        return true;
    }
    return false;
}

bool PolygonTool::onMouseRelease( const QPointF& pos ) {
    if (m_primitive) {
        m_primitive->moveLastPoint(pos);
        history()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool PolygonTool::onMouseMove( const QPointF& pos ) {
    if (Tool::onMouseMove(pos)) {
        m_primitive->moveLastPoint(pos);
        history()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool PolygonTool::beginCreating(const QPointF &pos) {
    Tool::beginCreating(pos);
    if (m_primitive)
        return false;
    m_primitive = new PolygonPrimitive( history()->scene(), pos );
    return true;
}

void PolygonTool::endCreating(const QPointF &pos) {
    Tool::endCreating(pos);
    if (m_primitive) {
        m_primitive->removeLast();
        if (m_primitive->pointsAmount()>2) {
            CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(m_primitive);
            m_primitive = 0;
            history()->appendCommand(cmd,true);
        } else {
            delete m_primitive;
            m_primitive = 0;
        }
    }
}
