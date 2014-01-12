#include "circletool.h"
#include "../primitives/circleprimitive.h"
#include "../commands/createprimitivecommand.h"
#include "../scene.h"
#include "../body.h"
#include "../canvas.h"
#include <cmath>

CircleTool::CircleTool(Scene *scene, QObject *parent) :
    Tool(scene,parent),m_primitive(0)
{
}


void CircleTool::Draw(const Canvas* canvas,QPainter* painter) const {
    if (!creating()) return;
    Body* b = scene()->activeBody();
    if (b)
        canvas->BeginDraw(b,painter);
    if (m_primitive)
        m_primitive->Draw( canvas, painter);
    if (b)
        canvas->EndDraw(b,painter);
}

bool CircleTool::onMouseMove( const QPointF& pos_ ) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    if (Tool::onMouseMove(pos)) {
        QPointF size = pos-beginPos();
        qreal r = sqrt(size.x()*size.x()+size.y()*size.y());
        m_primitive->setR( r );
        scene()->setText(m_primitive->text());
        return true;
    }
    return false;
}

bool CircleTool::beginCreating(const QPointF &pos_) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    Tool::beginCreating(pos);
    if (m_primitive)
        delete m_primitive;
    m_primitive = 0;
    m_primitive = new CirclePrimitive( scene()->activeBody(), pos, 5.0 );
    return true;
}

void CircleTool::endCreating(const QPointF &pos_) {
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (b) {
        pos = b->toLocal(pos_);
    }
    Tool::endCreating(pos);
    QPointF size = pos-beginPos();
    qreal r = sqrt(size.x()*size.x()+size.y()*size.y());
    m_primitive->setR( r );
    if (r<=0.0) {
        delete m_primitive;
        m_primitive = 0;
        return;
    }

    CreatePrimitiveCommand* cmd = new CreatePrimitiveCommand(scene()->activeBody(),m_primitive);
    m_primitive = 0;
    scene()->execCommand(cmd);

}
