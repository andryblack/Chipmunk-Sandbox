#include "boxtool.h"
#include "../primitives/boxprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../scene.h"
#include "../canvas.h"
#include "../body.h"

BoxTool::BoxTool(Scene *scene,QObject *parent) :
    Tool(scene,parent), m_primitive(0)
{
}

void BoxTool::Draw(const Canvas* canvas,QPainter* painter) const {
    if (!creating()) return;
    Body* b = scene()->activeBody();
    if (b)
        canvas->BeginDraw(b,painter);
    if (m_primitive)
        m_primitive->Draw(canvas,painter);
    if (b)
        canvas->EndDraw(b,painter);
}

bool BoxTool::onMouseMove(const QPointF &pos_) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    if (Tool::onMouseMove(pos)) {
        QPointF size = pos-beginPos();
        if (size.x()>0)
            m_primitive->setRight(pos.x());
        else
            m_primitive->setLeft(pos.x());
        if (size.y()>0)
            m_primitive->setBottom(pos.y());
        else
            m_primitive->setTop(pos.y());
        scene()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool BoxTool::beginCreating(const QPointF &pos_) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    Tool::beginCreating(pos);
    QRect rect;
    rect.setX(pos.x());
    rect.setY(pos.y());
    rect.setWidth(10);
    rect.setHeight(10);
    if (m_primitive)
        delete m_primitive;
    m_primitive = 0;
    m_primitive = new BoxPrimitive( scene()->activeBody(),rect );
    return true;
}

void BoxTool::endCreating(const QPointF &pos_) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    Tool::endCreating(pos);
    QPointF size = pos-beginPos();
    if (size.x()>0)
        m_primitive->setRight(pos.x());
    else if (size.x()<0)
        m_primitive->setLeft(pos.x());
    else {
        delete m_primitive;
        m_primitive = 0;
        return;
    }
    if (size.y()>0)
        m_primitive->setBottom(pos.y());
    else if (size.y()<0)
        m_primitive->setTop(pos.y());
    else {
        delete m_primitive;
        m_primitive = 0;
        return;
    }

    CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(scene()->activeBody(), m_primitive);
    m_primitive = 0;
    scene()->execCommand(cmd);
}
