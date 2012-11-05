#include "scene.h"
#include "primitive.h"
#include "command.h"
#include "history.h"
#include "body.h"
#include <QIcon>
#include <QFont>
#include <QBrush>
#include <QDomDocument>
#include <QFile>
#include <QMetaProperty>
#include <QSettings>

Scene::Scene(History *history, QObject *parent) :
    QAbstractItemModel(parent), m_history(history), m_zoom(1.0)
{
    m_zoom = 1.0f;
    m_active_body = 0;
    m_static_body = 0;
}

void Scene::makeNew(const QSize& size) {
    clearSelection();
    beginResetModel();
    m_worldSize = size;

    if (m_static_body)
        delete m_static_body;
    m_static_body = 0;
    foreach( Body* b, m_bodys ) {
        delete b;
    }
    m_bodys.clear();
    m_static_body = new StaticBody(this,"static",this);
    connect( m_static_body,SIGNAL(changed()),this,SLOT(bodyChanged()));
    m_history->clear();
    endResetModel();
    m_filename.clear();
    emit changed();

}

void ReadAttributes(QObject* obj,QDomElement& el) {
    QDomElement prop = el.firstChildElement("property");
    while (prop.isElement()) {
        QString propName = prop.attribute("name");
        int propIndx = obj->metaObject()->indexOfProperty(propName.toAscii().constData());
        if (propIndx>=0 && propIndx<obj->metaObject()->propertyCount()) {
            QMetaProperty metaProperty(obj->metaObject()->property(propIndx));
            QString valueStr = prop.attribute("value");
            if (metaProperty.type()==QVariant::String) {
                metaProperty.write(obj,valueStr);
            } else if (metaProperty.type()==QVariant::Int) {
                metaProperty.write(obj,valueStr.toInt());
            } else if (metaProperty.type()==QVariant::UInt) {
                metaProperty.write(obj,valueStr.toUInt());
            } else if (metaProperty.type()==QVariant::Double) {
                metaProperty.write(obj,valueStr.toDouble());
            } else if (metaProperty.type()==QVariant::Bool) {
                metaProperty.write(obj,bool(valueStr.toInt()));
            } else if (metaProperty.type()==QVariant::Point) {
                QStringList sl = valueStr.split(";");
                metaProperty.write(obj,QPoint(sl.first().toInt(),sl.last().toInt()));
            } else if (metaProperty.type()==QVariant::PointF) {
                QStringList sl = valueStr.split(";");
                metaProperty.write(obj,QPoint(sl.first().toDouble(),sl.last().toDouble()));
            } else if (metaProperty.type()==QVariant::Size) {
                QStringList sl = valueStr.split(";");
                metaProperty.write(obj,QSize(sl.first().toInt(),sl.last().toInt()));
            } else if (metaProperty.type()==QVariant::SizeF) {
                QStringList sl = valueStr.split(";");
                metaProperty.write(obj,QSizeF(sl.first().toDouble(),sl.last().toDouble()));
            }
        }
        prop = prop.nextSiblingElement("property");
    }
}

bool Scene::load(const QString& fn) {
    QFile file(fn);
     if (!file.open(QIODevice::ReadOnly)) {
         return false;
     }

     QByteArray data = file.readAll();
     QDomDocument doc;
     if (!doc.setContent(data)) {
         return false;
     }

     clearSelection();
     beginResetModel();
     m_history->clear();
     if (m_static_body)
         delete m_static_body;
     m_static_body = 0;
     foreach( Body* b, m_bodys ) {
         delete b;
     }
     m_bodys.clear();

    QDomElement root = doc.documentElement();
    m_worldSize.setWidth(root.attribute("width").toInt());
    m_worldSize.setHeight(root.attribute("height").toInt());
    QDomElement node = root.firstChildElement("body");
    while (node.isElement()) {
        QString type = node.attribute("type");
        Body* b = 0;
        if (type=="StaticBody") {
            b = new StaticBody(this,"body",this);
            ReadAttributes(b,node);
            if (!m_static_body)
                m_static_body = b;
            else {
                m_bodys.push_back( b );
            }
        } else if (type=="DynamicBody") {
            b = new DynamicBody(this,"body",this);
            ReadAttributes(b,node);
            m_bodys.push_back( b );
        }
        if (b) {
            QDomElement primNode = node.firstChildElement("primitive");
            while (primNode.isElement()) {
                Primitive* p = 0;
                QString type = primNode.attribute("type");
                p = Primitive::create(type,b,this);
                if (p) {
                    ReadAttributes(p,primNode);
                    b->addPrimitive(p);
                }
                primNode = primNode.nextSiblingElement("primitive");
            }
            connect( b,SIGNAL(changed()),this,SLOT(bodyChanged()));
        }
        node = node.nextSiblingElement("body");
    }
    endResetModel();
    m_filename = fn;
    emit changed();
    return true;
}

void Scene::save() {
    save(m_filename);
}

static void WriteAttributes(QObject* obj,QDomElement& el,QDomDocument& doc) {
    el.setAttribute("type",obj->metaObject()->className());
    for(int i = 0; i < obj->metaObject()->propertyCount(); ++i) {
        QMetaProperty metaProperty(obj->metaObject()->property(i));
        {
            QDomElement prop = doc.createElement("property");
            prop.setAttribute("name",QString(metaProperty.name()));
            QVariant value = metaProperty.read(obj);
            QString strValue;
            if (metaProperty.type()==QVariant::String) {
                strValue = value.toString();
            } else if (metaProperty.type()==QVariant::Int) {
                strValue = value.toString();
            } else if (metaProperty.type()==QVariant::UInt) {
                strValue = value.toString();
            } else if (metaProperty.type()==QVariant::Double) {
                strValue = value.toString();
            } else if (metaProperty.type()==QVariant::Bool) {
                strValue = value.toBool() ? "1" : "0";
            }else if (metaProperty.type()==QVariant::Point) {
                strValue = QString::number(value.toPoint().x())+";"
                        + QString::number(value.toPoint().y());
            } else if (metaProperty.type()==QVariant::PointF) {
                strValue = QString::number(value.toPointF().x())+";"
                        + QString::number(value.toPointF().y());
            } else if (metaProperty.type()==QVariant::Size) {
                strValue = QString::number(value.toSize().width())+";"
                        + QString::number(value.toSize().height());
            } else if (metaProperty.type()==QVariant::SizeF) {
                strValue = QString::number(value.toSizeF().width())+";"
                        + QString::number(value.toSizeF().height());
            } else {
                QByteArray ar;
                QDataStream ds(&ar,QIODevice::WriteOnly);
                ds << value;
                strValue = ar.toBase64().data();
            }
            prop.setAttribute("value",strValue);
            el.appendChild(prop);
        }
    }
}

void Scene::save(const QString& fn) {
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"utf-8\"" ));
    QDomElement root = doc.createElement("scene");
    doc.appendChild(root);
    root.setAttribute("width",m_worldSize.width());
    root.setAttribute("height",m_worldSize.height());
    for (int i=0;i<bodysCount();++i) {
        Body* b = body(i);
        QDomElement be = doc.createElement("body");
        WriteAttributes(b,be,doc);
        for (int j=0;j<b->primitivesCount();++j) {
            Primitive* p = b->primitive(j);
            QDomElement pe = doc.createElement("primitive");
            WriteAttributes(p,pe,doc);
            be.appendChild(pe);
        }
        root.appendChild(be);
    }
    QFile file(fn);
     if (file.open(QIODevice::WriteOnly)) {
         file.write(doc.toByteArray());
         m_history->markSaved();
         emit changed();
     } else {

     }
}

void Scene::addBody( Body* b ) {
    emit layoutAboutToBeChanged();
    connect( b,SIGNAL(changed()),this,SLOT(bodyChanged()));
    m_bodys.push_back( b );
    b->setParent(this);
    emit changed();
    emit layoutChanged();
}

void Scene::removeBody( Body* b ) {
    emit layoutAboutToBeChanged();
    m_bodys.removeAll(b);
    emit changed();
    emit layoutChanged();
}

void Scene::bodyChanged() {
    emit layoutAboutToBeChanged();
    emit changed();
    emit layoutChanged();
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
    m_static_body->Draw( canvas, painter );
    foreach( Body* b, m_bodys) {
        b->Draw( canvas, painter );
    }
}


Primitive* Scene::getPrimitiveAtPoint(const QPointF &pnt) {
    Body* b = activeBody();
    if (b) {
        Primitive* p = b->getPrimitiveAtPoint(pnt);
        if (p) return p;
    }
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
    m_body_selected.clear();
    m_active_body = 0;
    emit selectionChanged();
}

void Scene::setSelected(Primitive* p) {
    m_body_selected.clear();
    m_selected.clear();
    if (p) {
        m_selected.push_back(p);
        setActiveBody(p->body());
    }
    emit selectionChanged();
}

void Scene::setSelected(Body* b) {
    m_selected.clear();
    m_body_selected.clear();
    if (b) {
        m_body_selected.push_back(b);
        setActiveBody(b);
    }
    emit selectionChanged();
}

void Scene::addSelected(Primitive* p) {
    if (p) {
        m_selected.push_back(p);
        setActiveBody(p->body());
    }
    emit selectionChanged();
}

void Scene::removeSelected(Primitive* p) {
    m_selected.removeAll(p);
    emit selectionChanged();
}

void Scene::addSelected(Body* p) {
    m_body_selected.push_back(p);
    setActiveBody(p);
    emit selectionChanged();
}

void Scene::removeSelected(Body* p) {
    m_body_selected.removeAll(p);
    emit selectionChanged();
}

void Scene::setActiveBody(Body *b) {
    m_active_body = b;
    emit changed();
}

Primitive* Scene::selected() const {
    if (m_selected.size()==1)
        return m_selected.front();
    return 0;
}

bool Scene::selected(const Primitive* p) const {
    return qFind(m_selected.constBegin(),m_selected.constEnd(),p)!=m_selected.constEnd();
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
    return m_bodys.size()+(m_static_body?1:0);
}

/*Body* Scene::staticBody() {
    return m_static_body;
}*/

Body* Scene::body(int indx) const {
    if (indx==0) {
        return m_static_body;
    }
    return m_bodys.at(indx-1);
}

int Scene::bodyIndex( Body* b) const {
    if ( b == m_static_body ) return 0;
    int idx = m_bodys.indexOf(b);
    if (idx>=0) return idx + 1;
    return idx;
}



//// scene tree model

int Scene::columnCount(const QModelIndex &/*parent*/) const
 {
     return 1;
 }

 QVariant Scene::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role == Qt::DecorationRole) {
         return QIcon(static_cast<SceneTreeItem*>(index.internalPointer())->iconFile());
     }

     if (role == Qt::FontRole) {
         if ( static_cast<SceneTreeItem*>(index.internalPointer())->active() ) {
             QFont fnt = QFont();
             fnt.setBold(true);
             return fnt;
         }
     }

     if (role == Qt::BackgroundRole) {
         if ( static_cast<SceneTreeItem*>(index.internalPointer())->active() ) {
             return QBrush(QColor(0xFF,0xFE,0xA9));
         }
     }

     if (role != Qt::DisplayRole)
         return QVariant();

     return static_cast<SceneTreeItem*>(index.internalPointer())->name();
 }

 Qt::ItemFlags Scene::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant Scene::headerData(int /*section*/, Qt::Orientation /*orientation*/,
                                 int /*role*/) const
  {
      return QVariant();
  }

  QModelIndex Scene::index(int row, int column, const QModelIndex &parent)
              const
  {
      if (!hasIndex(row, column, parent))
          return QModelIndex();

      if (!parent.isValid()) {
          if (row<bodysCount())
            return createIndex(row,column,body(row));
          return QModelIndex();
      }
      SceneTreeItem* parentItem = static_cast<SceneTreeItem*>(parent.internalPointer());
      if (row<parentItem->childCount()) {
          return createIndex(row,column,parentItem->child(row));
      }
      return QModelIndex();
  }

  QModelIndex Scene::parent(const QModelIndex &index) const
  {
      if (!index.isValid())
          return QModelIndex();
      SceneTreeItem* obj = static_cast<SceneTreeItem*>(index.internalPointer());
      if (!obj)
        return QModelIndex();
      SceneTreeItem* parentItem = obj->parent();
      if (parentItem) {
          return createIndex(parentItem->indexOf(obj),0,parentItem);
      }
      return QModelIndex();
  }

  int Scene::rowCount(const QModelIndex &parent) const
  {
      if (!parent.isValid())
          return bodysCount();

      if (parent.column() > 0)
          return 0;

      SceneTreeItem* obj = static_cast<SceneTreeItem*>(parent.internalPointer());

      return obj ? obj->childCount() : 0;
  }
