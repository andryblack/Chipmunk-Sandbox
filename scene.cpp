#include "scene.h"
#include "primitive.h"
#include "command.h"
#include "history.h"

Scene::Scene(History *history, QObject *parent) :
    QObject(parent), m_history(history), m_zoom(1.0)
{
    m_zoom = 1.0f;
    m_worldSize = QSize(1024,768);
}

void Scene::setZoom( qreal zoom ) {
    m_zoom = zoom;
    emit zoomChanged();
}

bool Scene::zoomInAllowed() const {
    return m_zoom < 8.0f;
}
bool Scene::zoomOutAllowed() const {
    return m_zoom > 0.25f;
}
bool Scene::zoomActualAllowed() const {
    return m_zoom != 1.0f;
}
void Scene::incrementZoom() {
    setZoom(m_zoom*2);
}

void Scene::decrementZoom() {
    setZoom(m_zoom/2);
}

void Scene::actualZoom() {
    setZoom(1.0f);
}

void Scene::setText(const QString &text) {
    m_text = text;
    emit textChanged();
}

void Scene::Draw( const Canvas* canvas, QPainter* painter) const {
    foreach( Primitive* p, m_free_primtives) {
        if (m_selected.indexOf(p)==-1)
            p->Draw( canvas, painter );
    }
}

void Scene::DrawSelected( const Canvas* canvas, QPainter* painter) const {
    foreach( Primitive* p, m_selected) {
        p->Draw( canvas, painter );
    }
}

void Scene::DrawMarkers( const Canvas* canvas, QPainter* painter) const {
    if (m_selected.size()==1) {
        m_selected.front()->DrawMarkers(canvas,painter);
    }
}

Primitive* Scene::getPrimitiveAtPoint(const QPointF &pnt) {
    foreach( Primitive* p, m_free_primtives) {
        if ( p->isPointInside(pnt) )
            return p;
    }
    return 0;
}

void Scene::removePrimitive(Primitive *p) {
    m_free_primtives.removeAll(p);
    emit changed();
}

void Scene::appendPrimitive(Primitive *p) {
    p->setParent(this);
    m_free_primtives.push_back(p);
    emit changed();
}

void Scene::clearSelection() {
    m_selected.clear();
    emit changed();
}

void Scene::setSelected(Primitive* p) {
    m_selected.clear();
    m_selected.push_back(p);
    emit changed();
}

void Scene::addSelected(Primitive* p) {
    m_selected.push_back(p);
    emit changed();
}

Primitive* Scene::selected() const {
    if (m_selected.size()==1)
        return m_selected.front();
    return 0;
}

void Scene::update() {
    emit changed();
}

void Scene::execCommand( Command* cmd ) {
    if (cmd) {
        cmd->Execute(this);
        m_history->appendCommand(cmd);
    }
}

void Scene::undo() {
    m_history->undo( this );
}

void Scene::redo() {
    m_history->redo( this );
}
