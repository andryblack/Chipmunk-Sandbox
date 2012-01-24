#include "primitive.h"
#include "canvas.h"
#include "primitivemarker.h"
#include "scene.h"

Primitive::Primitive(Scene* scene,QObject *parent) :
    QObject(parent), m_scene(scene)
{
}

qreal   Primitive::sceneZoom() const {
    return m_scene->zoom();
}



