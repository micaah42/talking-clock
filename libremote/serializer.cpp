#include "serializer.h"

#include <QJsonObject>
#include <QLoggingCategory>
#include <QMetaProperty>
#include <QVariant>

#include "qjsonarray.h"
#include "templatemodel.h"

namespace {
Q_LOGGING_CATEGORY(self, "serializer")
}

VariantSerializer *VariantSerializer::I()
{
    if (_instance == nullptr)
        _instance = new VariantSerializer();
    return _instance;
}
VariantSerializer *VariantSerializer::_instance = nullptr;

VariantSerializer::VariantSerializer()
{
    // handle unserializable basic types
    _serializers[QMetaType::fromType<QDateTime>().id()] = [](const QVariant &variant) {
        return variant.value<QDateTime>().toString(Qt::ISODate);
    };
    _serializers[QMetaType::fromType<QTime>().id()] = [](const QVariant &variant) {
        return variant.value<QTime>().toString("HH:mm:ss");
    };
    _serializers[QMetaType::fromType<void>().id()] = [](const QVariant &variant) {
        return QJsonValue::Null;
    };

    // handle unserializable basic types
    _deserializers[QMetaType::fromType<QDateTime>().id()] = [](const QJsonValue &value) {
        return QVariant::fromValue(QDateTime::fromString(value.toString(), Qt::ISODate));
    };
    _deserializers[QMetaType::fromType<QTime>().id()] = [](const QJsonValue &value) {
        return QVariant::fromValue(QTime::fromString(value.toString(), "HH:mm:ss"));
    };
}

QJsonValue VariantSerializer::serialize(const QVariant &variant)
{
    if (!_serializers.contains(variant.userType())) {
        _serializers[variant.userType()] = registerSerializer(variant.userType());
    }

    auto value = _serializers[variant.userType()](variant);
    return value;
}

QVariant VariantSerializer::deserialize(const int typeId, const QJsonValue &value)
{
    if (!_deserializers.contains(typeId)) {
        _deserializers[typeId] = registerDeserializer(typeId);
    }

    auto variant = _deserializers[typeId](value);
    return variant;
}

std::function<QJsonValue(const QVariant &)> VariantSerializer::registerSerializer(int metaTypeId)
{
    QMetaType metaType(metaTypeId);
    qCInfo(self) << "registering serializer:" << metaType.name() << metaType.flags();

    auto metaObject = metaType.metaObject();

    // handle basic types
    if (metaObject == nullptr) {
        return [](const QVariant &variant) { return QJsonValue::fromVariant(variant); };
    }

    // handle qobject type variants
    if (metaType.flags().testFlag(QMetaType::PointerToQObject)) {
        return [this, metaObject](const QVariant &variant) {
            QJsonObject object;
            auto qobject = variant.value<QObject *>();

            // serialize properties
            for (int i = 0; i < metaObject->propertyCount(); i++) {
                auto property = metaObject->property(i);
                auto value = property.read(qobject);
                object[property.name()] = this->serialize(value);
            }

            // serialize model items if object is of model type
            if (metaObject->inherits(QMetaType::fromType<TemplateModel *>().metaObject())) {
                QJsonArray array;
                auto model = qobject_cast<TemplateModel *>(qobject);
                for (int i = 0; i < model->size(); ++i) {
                    auto value = model->item(i);
                    array.append(this->serialize(value));
                }
                object["#"] = array;
            }
            return object;
        };
    }

    // handle qgadgets
    if (metaType.flags().testFlag(QMetaType::IsGadget)) {
        return [this, metaObject](const QVariant &variant) {
            QJsonObject object;
            auto gadget = variant.data();
            for (int i = 0; i < metaObject->propertyCount(); i++) {
                auto property = metaObject->property(i);
                auto value = property.readOnGadget(gadget);
                object[property.name()] = this->serialize(value);
            }
            return object;
        };
    }

    qFatal("Failed to build serializer!");
}

std::function<QVariant(const QJsonValue &)> VariantSerializer::registerDeserializer(int typeId)
{
    QMetaType metaType(typeId);
    qCInfo(self) << "register deserializer:" << metaType.name() << metaType.flags();

    auto metaObject = metaType.metaObject();

    // handle basic types
    if (metaObject == nullptr) {
        return [](const QJsonValue &value) { return value.toVariant(); };
    }

    // handle qobjects
    if (metaType.flags().testFlag(QMetaType::PointerToQObject)) {
        qFatal("Deserialization into QObjects not implemented yet");
    }

    // handle qgagets
    if (metaType.flags().testFlag(QMetaType::IsGadget)) {
        return [this, metaObject, typeId](const QJsonValue &value) {
            void *gadget = QMetaType::create(typeId);
            auto object = value.toObject();
            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                auto property = metaObject->property(i);
                auto propertyType = property.userType();
                auto propertyValue = object[property.name()];
                property.writeOnGadget(gadget, this->deserialize(propertyType, propertyValue));
            }
            return QVariant(typeId, const_cast<void *>(gadget));
        };
    }

    qFatal("Failed to build deserializer!");
}
