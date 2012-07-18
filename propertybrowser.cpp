/* 
 * File:   PropertyBrowser.cpp
 * Author: mark
 * 
 * Created on August 23, 2009, 10:29 PM
 */

#include <QMetaProperty>
#include <QDateTime>
#include "propertybrowser.h"
#include "commands/properychangedcommand.h"


PropertyBrowser::PropertyBrowser(QWidget* parent)
    : QtTreePropertyBrowser(parent)
    , m_history(0)
    , m_variantManager(new QtVariantPropertyManager(this))
    , m_variantEditorFactory(new QtVariantEditorFactory(this))
{
    //setHeaderVisible(false);
    //setPropertiesWithoutValueMarked(true);
    //setIndentation(10);
    //setResizeMode(ResizeToContents);
    //setAlternatingRowColors(false);
    setFactoryForManager(m_variantManager, new QtVariantEditorFactory);
    
}

void PropertyBrowser::valueChanged(QtProperty *property, const QVariant &value)
{
    if(m_propertyMap.find(property) != m_propertyMap.end()) { 
        foreach(SceneTreeItem *obj, m_selectedObjects) {
            const char* prop = m_propertyMap[property];
            if (m_history) {
                m_history->appendCommand( new ProperyChangedCommand(
                                              obj,
                                              prop,
                                              obj->property(prop),
                                              value) );
            }
            obj->setProperty(prop, value);
        }
    }
}

QString PropertyBrowser::humanize(QString str) const
{
    return str.at(0).toUpper() + str.mid(1).replace(QRegExp("([a-z])([A-Z])"), "\\1 \\2");
}

void PropertyBrowser::setSelectedObjects(const QList<SceneTreeItem *> objs)
{
    foreach(SceneTreeItem *obj, m_selectedObjects) {
        disconnect(obj, SIGNAL(propertyChanged()), this, SLOT(objectUpdated()));
    }
    clear();
    m_variantManager->clear();
    m_selectedObjects = objs;
    m_propertyMap.clear();
    if(objs.isEmpty()) {
        return;
    }
    for(int i = 0; i < objs.first()->metaObject()->propertyCount(); ++i) { // FIXME: this loop takes ~250 ms for 18 properties
        QMetaProperty metaProperty(objs.first()->metaObject()->property(i));
        if(metaProperty.isDesignable()) {
            QtProperty *property = m_variantManager->addProperty(metaProperty.type(), humanize(metaProperty.name()));
            property->setEnabled(metaProperty.isWritable());
            m_propertyMap[property] = metaProperty.name();
            addProperty(property);
            setExpanded(topLevelItem(property), false);
        }
    }
    foreach(SceneTreeItem *obj, m_selectedObjects) {
        connect(obj, SIGNAL(propertyChanged()), this, SLOT(objectUpdated()));
    }
    objectUpdated();
    
}

void PropertyBrowser::objectUpdated()
{
    if(m_selectedObjects.isEmpty()) {
        return;
    }
    disconnect(m_variantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), 
            this, SLOT(valueChanged(QtProperty*, QVariant)));
    QMapIterator<QtProperty*, QByteArray> i(m_propertyMap); // FIXME: should be looping over object's properties, not property map
    bool diff;
    while(i.hasNext()) {
        i.next();
        diff = false;
        for(int j = 1; j < m_selectedObjects.size(); ++j) {
            if(m_selectedObjects.at(j)->property(i.value()) != m_selectedObjects.at(j - 1)->property(i.value())) {
                diff = true;
                break;
            }
        }
        if(diff) setBackgroundColor(topLevelItem(i.key()), QColor(0xFF,0xFE,0xA9));
        else setBackgroundColor(topLevelItem(i.key()), Qt::white);
        m_variantManager->setValue(i.key(), m_selectedObjects.first()->property(i.value()));
    }
    connect(m_variantManager, SIGNAL(valueChanged(QtProperty*, QVariant)), 
            this, SLOT(valueChanged(QtProperty*, QVariant)));
}
