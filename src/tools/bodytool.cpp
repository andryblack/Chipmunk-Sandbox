#include "bodytool.h"
#include "../scene.h"
#include "../canvas.h"
#include "../body.h"
#include "../primitive.h"
#include "../commands/movebodycommand.h"

BodyTool::BodyTool(Scene *scene, QObject *parent) :
    Tool(scene,parent)
{
}

void BodyTool::Activate()
{
    m_moved_body = 0;
    Tool::Activate();
    scene()->setSelected(static_cast<Primitive*>(0));
}

void BodyTool::Draw(const Canvas* canvas,QPainter* painter) const {
    Body* body = scene()->activeBody();
    if (body) {
        canvas->Draw(body,painter);
    }
}

bool BodyTool::onMousePress(const QPointF &pos)
{
    m_moved_body = 0;
    Body* body = scene()->activeBody();
    Primitive* primitive = scene()->getPrimitiveAtPoint( pos );

    if (primitive) {
        Body* pbody = primitive->body();
        if (pbody != body) {
            scene()->setSelected(pbody);
        } else {
            m_start_pos = body->position();
            m_offset = m_start_pos - pos;
            m_moved_body = body;
        }
    }
    return true;
}

bool BodyTool::onMouseMove(const QPointF &pos)
{
    if (m_moved_body) {
        QPointF newPos = pos + m_offset;
        m_moved_body->setPosition(newPos);
    }

    return true;
}

bool BodyTool::onMouseRelease(const QPointF &pos)
{
    if (m_moved_body) {
        QPointF newPos = pos + m_offset;
        m_moved_body->setPosition(newPos);
        if (newPos != m_start_pos) {
            MoveBodyCommand* cmd = new MoveBodyCommand(m_moved_body,m_start_pos,newPos);
            scene()->execCommand(cmd);
        }
        m_moved_body = 0;
    }
    return true;
}
