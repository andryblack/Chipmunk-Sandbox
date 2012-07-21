#include "edittool.h"
#include "../scene.h"
#include "../primitive.h"
#include "../commands/moveprimitivecommand.h"
#include "../primitivemarker.h"
#include "../body.h"

EditTool::EditTool(Scene *scene,QObject *parent) :
    Tool(scene,parent)
{
    m_selected = 0;
    m_selected_marker = 0;
}

void EditTool::Activate() {
    Tool::Activate();
    m_selected = 0;
    m_selected_marker = 0;
    m_moved = false;
}

bool EditTool::onMousePress( const QPointF& pos_ ) {
    m_selected = scene()->selected();
    Body* b = scene()->activeBody();
    QPointF pos = pos_;
    if (m_selected) {
        if (b) {
            pos = b->toLocal(pos_);
        }
        PrimitiveMarker* marker = m_selected->getMarkerAtPoint(pos);
        if (marker) {
            m_start_pos = marker->position();
            m_offset = marker->getOffset( pos );
            m_selected_marker = marker;
            m_selected_marker->activate();
            m_moved = false;
            return true;
        }
    }
    m_selected_marker = 0;
    pos = pos_;
    Primitive* primitive = scene()->getPrimitiveAtPoint( pos );
    if (b) {
        pos = b->toLocal(pos_);
    }
    if (primitive) {
        scene()->setSelected( primitive );
        m_start_pos = primitive->position();
        m_offset = m_start_pos - pos;
    } else {
        scene()->clearSelection();
        scene()->setText("");
    }
    m_moved = false;
    m_selected = primitive;
    return true;
}

bool EditTool::onMouseMove(const QPointF &pos_) {
    if (m_selected) {
        QPointF pos = pos_;
        Body* b = scene()->activeBody();
        if (b) {
            pos = b->toLocal(pos_);
        }

        if ( m_selected_marker ) {
            m_selected_marker->move( pos+m_offset );
        } else {
            m_selected->move( pos+m_offset );
            m_moved = m_selected->position()!=m_start_pos;
        }
        scene()->setText(m_selected->text());
        return true;
    }
    return false;
}

bool EditTool::onMouseRelease(const QPointF &pos_) {
    if (m_selected) {
       {
            QPointF pos = pos_;
            Body* b = scene()->activeBody();
            if (b) {
                pos = b->toLocal(pos_);
            }
            if ( m_selected_marker ) {
                m_selected_marker->move(pos+m_offset);
                m_selected_marker->complete(scene());
                m_selected_marker = 0;
            } else if ( m_moved ){
                MovePrimitiveCommand* cmd = new MovePrimitiveCommand(m_selected,m_start_pos,pos+m_offset);
                scene()->execCommand(cmd);
            }
        }
        scene()->setText(m_selected->text());
        m_selected = 0;
    }
    return true;
}
