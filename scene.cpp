#include "scene.h"
#include "primitive.h"
#include "command.h"
#include "history.h"
#include "body.h"

Scene::Scene(History *history, QObject *parent) :
    QObject(parent), m_history(history), m_zoom(1.0)
{
    m_zoom = 1.0f;
    m_worldSize = QSize(1024,768);

    m_static_body = new StaticBody(this,"static",this);
    connect( m_static_body,SIGNAL(changed()),this,SLOT(bodyChanged()));
}

void Scene::addBody( Body* b ) {
    connect( b,SIGNAL(changed()),this,SLOT(bodyChanged()));
    m_bodys.push_back( b );
}

void Scene::bodyChanged() {
    emit changed();
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
    m_static_body->Draw( canvas, painter, m_selected );
    foreach( Body* b, m_bodys) {
        b->Draw( canvas, painter, m_selected );
    }
}

void Scene::DrawSelected( const Canvas* canvas, QPainter* painter) const {
    foreach( Primitive* p, m_selected) {
        Body* b = p->body();
        if ( b ) {
            b->Draw( canvas, painter, p );
        }
    }
}

void Scene::DrawMarkers( const Canvas* canvas, QPainter* painter) const {
    if (m_selected.size()==1) {
        m_selected.front()->DrawMarkers(canvas,painter);
    }
}

Primitive* Scene::getPrimitiveAtPoint(const QPointF &pnt) {
    Primitive* p = m_static_body->getPrimitiveAtPoint(pnt);
    if (p) return p;

    foreach( Body* b, m_bodys) {
        p = b->getPrimitiveAtPoint(pnt);
        if (p) {
            return p;
        }
    }
    return 0;
}



void Scene::clearSelection() {
    m_selected.clear();
    emit selectionChanged();
}

void Scene::setSelected(Primitive* p) {
    m_selected.clear();
    m_selected.push_back(p);
    emit selectionChanged();
}

void Scene::addSelected(Primitive* p) {
    m_selected.push_back(p);
    emit selectionChanged();
}

void Scene::removeSelected(Primitive* p) {
    m_selected.removeAll(p);
    emit selectionChanged();
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

int Scene::bodysCount() const {
    return m_bodys.size()+1;
}

Body* Scene::staticBody() {
    return m_static_body;
}

Body* Scene::body(int indx) {
    if (indx==0) {
        return m_static_body;
    }
    return m_bodys.at(indx-1);
}
