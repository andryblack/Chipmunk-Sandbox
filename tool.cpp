#include "tool.h"
#include "scene.h"

Tool::Tool(Scene* scene,QObject *parent) :
    QObject(parent) , m_scene(scene)
{
    m_creating = false;
}

void Tool::Draw(const Canvas* ,QPainter* ) const {

}

void Tool::Activate() {
    scene()->clearSelection();
}


bool Tool::creating() const {
    return m_creating;
}

bool Tool::beginCreating( const QPointF& pos ) {
    m_begin_pos = pos;
    return false;
}

void Tool::endCreating(const QPointF &/*pos*/) {
    m_creating = false;
}

bool Tool::onMousePress( const QPointF& pos ) {
    if (beginCreating(pos)) {
        m_creating = true;
        return true;
    }
    return false;
}

bool Tool::onMouseMove( const QPointF&/*pos*/ ) {
    return creating();
}

bool Tool::onMouseRelease( const QPointF& pos ) {
    if (creating()) {
        endCreating(pos);
        return true;
    }
    return false;
}
