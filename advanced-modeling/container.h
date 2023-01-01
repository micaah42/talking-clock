#ifndef CONTAINIER_H
#define CONTAINIER_H

#include <QObject>

#include "serializable.h"
#include "templatemodel.h"

DECLARE_MODEL(Serializable)

class Container : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(SerializableModel *model READ model)
    SerializableModel *model() { return &_model; };

private:
    SerializableModel _model;
};

#endif // CONTAINIER_H
