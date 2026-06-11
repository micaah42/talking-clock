#ifndef KEYVALUEOBJECT_H
#define KEYVALUEOBJECT_H

#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>

class KeyValueObject : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString key READ key WRITE setKey NOTIFY keyChanged FINAL)
    Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
    explicit KeyValueObject(QObject *parent = nullptr);

    QString key() const;
    void setKey(const QString &newKey);

    QString value() const;
    void setValue(const QString &newValue);

signals:
    void valueChanged();
    void keyChanged();

private:
    QString _key;
    QString _value;
};

#endif // KEYVALUEOBJECT_H
