#include "polygontool.h"
#include "../scene.h"
#include "../primitives/polygonprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../canvas.h"
#include "../body.h"

PolygonTool::PolygonTool(Scene *scene, QObject *parent) :
    Tool(scene,parent),m_primitive(0)
{
}

void PolygonTool::Activate() {
    if (m_primitive) {
        delete m_primitive;
        m_primitive = 0;
    }
}

void PolygonTool::Draw(const Canvas* canvas,QPainter* painter) const {
    Body* b = scene()->activeBody();
    if (m_primitive) {
        if (b)
            canvas->BeginDraw(b,painter);
        canvas->Draw( m_primitive, painter, true );
        if (b)
            canvas->EndDraw(b,painter);
    }
}

bool PolygonTool::onMousePress( const QPointF& pos_ ) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
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

bool PolygonTool::onMouseRelease( const QPointF& pos_ ) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    if (m_primitive) {
        m_primitive->moveLastPoint(pos);
        scene()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool PolygonTool::onMouseMove( const QPointF& pos_ ) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    if (Tool::onMouseMove(pos)) {
        m_primitive->moveLastPoint(pos);
        scene()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool PolygonTool::beginCreating(const QPointF &pos) {
    Tool::beginCreating(pos);
    if (m_primitive)
        return false;
    m_primitive = new PolygonPrimitive( scene()->activeBody(), pos );
    return true;
}

void PolygonTool::endCreating(const QPointF &pos) {
    Tool::endCreating(pos);
    if (m_primitive) {
        m_primitive->removeLast();
        if (m_primitive->pointsAmount()>2) {
            CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(scene()->activeBody(),m_primitive);
            m_primitive = 0;
            scene()->execCommand(cmd);
        } else {
            delete m_primitive;
            m_primitive = 0;
        }
    }
}
