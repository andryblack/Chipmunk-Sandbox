#include "edittool.h"
#include "../history.h"
#include "../scene.h"
#include "../primitive.h"
#include "../commands/moveprimitivecommand.h"

EditTool::EditTool(History* history,QObject *parent) :
    Tool(history,parent)
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

bool EditTool::onMousePress( const QPointF& pos ) {
    m_selected = history()->scene()->selected();
    if (m_selected) {
        PrimitiveMarker* marker = m_selected->getMarkerAtPoint(pos);
        if (marker) {
            m_start_pos = marker->position();
            m_offset = m_start_pos - pos;
            m_selected_marker = marker;
            m_selected_marker->reset();
            m_moved = false;
            return true;
        }
    }
    m_selected_marker = 0;
    Scene* scene = history()->scene();
    Primitive* primitive = scene->getPrimitiveAtPoint( pos );
    if (primitive) {
        scene->setSelected( primitive );
        m_start_pos = primitive->position();
        m_offset = m_start_pos - pos;
    } else {
        scene->clearSelection();
        history()->setText("");
    }
    m_moved = false;
    m_selected = primitive;
    return true;
}

bool EditTool::onMouseMove(const QPointF &pos) {
    if (m_selected) {
        if ( m_selected_marker ) {
            m_selected_marker->move( pos+m_offset );
        } else {
            m_selected->move( pos+m_offset );
            m_moved = m_selected->position()!=m_start_pos;
        }
        history()->setText(m_selected->text());
        return true;
    }
    return false;
}

bool EditTool::onMouseRelease(const QPointF &pos) {
    if (m_selected) {
       {
            if ( m_selected_marker ) {
                m_selected_marker->move(pos+m_offset);
                Command* cmd = m_selected_marker->generateCommand();
                if (cmd) history()->appendCommand(cmd,true);
                m_selected_marker->reset();
                m_selected_marker = 0;
            } else if ( m_moved ){
                MovePrimitiveCommand* cmd = new MovePrimitiveCommand(m_selected,m_start_pos,pos+m_offset);
                history()->appendCommand(cmd,true);
            }
        }
        history()->setText(m_selected->text());
        m_selected = 0;
    }
    return true;
}
