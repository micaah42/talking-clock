#ifndef ADVANCEDMODEL_H
#define ADVANCEDMODEL_H

#include <QJsonValue>
#include <QMap>
#include <QMetaProperty>
#include <QMetaType>
#include <QObject>

#include "libremote_global.h"

class LIBREMOTE_EXPORT VariantSerializer
{
public:
    static VariantSerializer *I();

    QJsonValue serialize(const QVariant &variant);

    template<class T>
    QJsonValue serialize(const T &value)
    {
        return this->serialize(QVariant::fromValue(value));
    }

    template<class T>
    T deserialize(const QJsonValue &value)
    {
        const QVariant variant = this->deserialize(QMetaType::fromType<T>().id(), value);
        return variant.value<T>();
    }

    QVariant deserialize(const int typeId, const QJsonValue &object);

private:
    explicit VariantSerializer();
    static VariantSerializer *_instance;

    std::function<QJsonValue(const QVariant &)> buildSerializer(int metaType);
    std::function<QVariant(const QJsonValue &)> buildDeserializer(int metaType);

    QMap<int, std::function<QJsonValue(const QVariant &)>> _serializers;
    QMap<int, std::function<QVariant(const QJsonValue &)>> _deserializers;
};

#endif // ADVANCEDMODEL_H
