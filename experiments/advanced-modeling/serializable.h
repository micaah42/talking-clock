#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QJsonObject>

class Serializable
{
    Q_GADGET
    Q_PROPERTY(int integer READ integer WRITE setInteger)
    Q_PROPERTY(QString str READ string WRITE setString)

public:
    explicit Serializable(int integer = 0, QString string = "");
    int integer() const;
    void setInteger(int newInteger);
    const QString &string() const;
    void setString(const QString &newString);

private:
    int _integer;
    QString _string;
};
Q_DECLARE_METATYPE(Serializable)

#endif // SERIALIZABLE_H
