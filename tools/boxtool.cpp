#include "boxtool.h"
#include "../canvas.h"
#include "../primitives/boxprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../history.h"

BoxTool::BoxTool(History* history,QObject *parent) :
    Tool(history,parent), m_primitive(0)
{
}

void BoxTool::Draw(const Canvas* canvas,QPainter* painter) const {
    if (m_primitive)
        canvas->Draw(m_primitive,painter);
}

bool BoxTool::onMouseMove(const QPointF &pos) {
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
        history()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool BoxTool::beginCreating(const QPointF &pos) {
    Tool::beginCreating(pos);
    QRect rect;
    rect.setX(pos.x());
    rect.setY(pos.y());
    rect.setWidth(10);
    rect.setHeight(10);
    if (m_primitive)
        delete m_primitive;
    m_primitive = 0;
    m_primitive = new BoxPrimitive( history()->scene(),rect );
    return true;
}

void BoxTool::endCreating(const QPointF &pos) {
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

    CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(m_primitive);
    m_primitive = 0;
    history()->appendCommand(cmd,true);

}
