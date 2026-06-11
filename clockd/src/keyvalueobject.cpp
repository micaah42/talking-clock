#include "keyvalueobject.h"

KeyValueObject::KeyValueObject(QObject *parent)
    : QObject{parent}
{}

QString KeyValueObject::value() const
{
    return _value;
}

void KeyValueObject::setValue(const QString &newValue)
{
    if (_value == newValue)
        return;

    _value = newValue;
    emit valueChanged();
}

QString KeyValueObject::key() const
{
    return _key;
}

void KeyValueObject::setKey(const QString &newKey)
{
    if (_key == newKey)
        return;

    _key = newKey;
    emit keyChanged();
}
