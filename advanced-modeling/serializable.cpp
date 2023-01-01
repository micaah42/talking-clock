#include "serializable.h"

Serializable::Serializable(int integer, QString string) : _integer{integer}, _string{string} {}

int Serializable::integer() const
{
    return _integer;
}

void Serializable::setInteger(int newInteger)
{
    _integer = newInteger;
}

const QString &Serializable::string() const
{
    return _string;
}

void Serializable::setString(const QString &newString)
{
    _string = newString;
}
