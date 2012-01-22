#include "circletool.h"
#include "../primitives/circleprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../history.h"

#include <cmath>

CircleTool::CircleTool(History *history, QObject *parent) :
    Tool(history,parent),m_primitive(0)
{
}


void CircleTool::Draw(const Canvas* canvas,QPainter* painter) const {
    if (m_primitive)
        m_primitive->Draw( canvas, painter);
}

bool CircleTool::onMouseMove( const QPointF& pos ) {
    if (Tool::onMouseMove(pos)) {
        QPointF size = pos-beginPos();
        qreal r = sqrt(size.x()*size.x()+size.y()*size.y());
        m_primitive->setR( r );
        history()->setText(m_primitive->text());
        return true;
    }
}

bool CircleTool::beginCreating(const QPointF &pos) {
    Tool::beginCreating(pos);
    if (m_primitive)
        delete m_primitive;
    m_primitive = 0;
    m_primitive = new CirclePrimitive( history()->scene(), pos, 5.0 );
    return true;
}

void CircleTool::endCreating(const QPointF &pos) {
    Tool::endCreating(pos);
    QPointF size = pos-beginPos();
    qreal r = sqrt(size.x()*size.x()+size.y()*size.y());
    m_primitive->setR( r );
    if (r<=0.0) {
        delete m_primitive;
        m_primitive = 0;
        return;
    }

    CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(m_primitive);
    m_primitive = 0;
    history()->appendCommand(cmd,true);

}
