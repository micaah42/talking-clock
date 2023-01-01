#ifndef JSONSPECIFICATION_H
#define JSONSPECIFICATION_H

#include <QJsonObject>
#include <QMap>

class JsonSpec
{
public:
    JsonSpec(std::initializer_list<std::pair<QString, QJsonValue::Type>> list);
    void set(const QString &key, const QJsonValue::Type type);
    bool check(const QJsonObject &obj);

private:
    QMap<QString, QJsonValue::Type> _specs;
};

#endif // JSONSPECIFICATION_H
