#ifndef PROPERYCHANGEDCOMMAND_H
#define PROPERYCHANGEDCOMMAND_H

#include "../command.h"
#include <QVariant>

class ProperyChangedCommand : public Command
{
    Q_OBJECT
public:
    explicit ProperyChangedCommand( QObject* obj,
                                    const char *prop,
                                   const QVariant& from,
                                   const QVariant& to,
                                   QObject *parent = 0);
    
    virtual void Execute(Scene* scene) ;
    virtual void Undo(Scene* scene) ;
    virtual void Redo(Scene* scene) ;

    virtual QString text() const;
signals:
    
public slots:
private:
    QObject*    m_obj;
    QByteArray     m_prop;
    QVariant    m_from;
    QVariant    m_to;
};

#endif // PROPERYCHANGEDCOMMAND_H
