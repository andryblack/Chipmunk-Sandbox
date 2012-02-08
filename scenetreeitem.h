#ifndef SCENETREEITEM_H
#define SCENETREEITEM_H

#include <QObject>

class SceneTreeItem : public QObject
{
    Q_OBJECT
public:
    explicit SceneTreeItem(QObject *parent = 0);
    
    virtual QString name() const = 0;

    virtual int childCount() const = 0;
    virtual SceneTreeItem* child(int indx) = 0;
    virtual int indexOf(SceneTreeItem* child) = 0;
    virtual SceneTreeItem* parent() const = 0;
    virtual QString iconFile() const { return "empty"; }
signals:
    
public slots:
    
};

#endif // SCENETREEITEM_H
