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

    _serializers[QMetaType::fromType<QDateTime>().id()] = [](const QVariant &variant) { return variant.value<QDateTime>().toString(Qt::ISODate); };
    _serializers[QMetaType::fromType<QTime>().id()] = [](const QVariant &variant) { return variant.value<QTime>().toString("HH:mm"); };
    _serializers[QMetaType::fromType<void>().id()] = [](const QVariant &variant) { return QJsonValue::Null; };

    // handle unserializable basic types
    _deserializers[QMetaType::fromType<QDateTime>().id()] = [](const QJsonValue &value) {
        return QVariant::fromValue(QDateTime::fromString(value.toString(), Qt::ISODate));
    };
    _deserializers[QMetaType::fromType<QTime>().id()] = [](const QJsonValue &value) {
        return QVariant::fromValue(QTime::fromString(value.toString(), "HH:mm"));
    };
}

QJsonValue VariantSerializer::serialize(const QVariant &variant)
{
    if (!_serializers.contains(variant.userType())) {
        _serializers[variant.userType()] = buildSerializer(variant.userType());
    }

    auto value = _serializers[variant.userType()](variant);
    return value;
}

QVariant VariantSerializer::deserialize(const int typeId, const QJsonValue &value)
{
    if (!_deserializers.contains(typeId)) {
        _deserializers[typeId] = buildDeserializer(typeId);
    }

    auto variant = _deserializers[typeId](value);
    return variant;
}

std::function<QJsonValue(const QVariant &)> VariantSerializer::buildSerializer(int metaTypeId)
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
                auto serializedValue = this->serialize(value);
                object[property.name()] = serializedValue;
                qCDebug(self) << "serialized" << property.typeName() << property.name() << serializedValue;
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

std::function<QVariant(const QJsonValue &)> VariantSerializer::buildDeserializer(int typeId)
{
    QMetaType metaType(typeId);
    qCInfo(self) << "register deserializer:" << metaType.name() << metaType.flags();

    auto metaObject = metaType.metaObject();

    if (metaObject == nullptr) {
        return [](const QJsonValue &value) { return value.toVariant(); };
    }

    if (metaType.flags().testFlag(QMetaType::PointerToQObject)) {
        if (!metaType.isDefaultConstructible())
            qCCritical(self) << "cannot create serializer for" << metaType.name() << "(default)";
        if (!metaType.isMoveConstructible())
            qCCritical(self) << "cannot create serializer for" << metaType.name() << "(move)";
        if (!metaType.isCopyConstructible())
            qCCritical(self) << "cannot create serializer for" << metaType.name() << "(copy)";

        return [this, metaObject, typeId](const QJsonValue &value) {
            static QMetaType metaType{typeId};

            if (!value.isObject()) {
                qCCritical(self) << "expected object for:" << metaType.name() << "got" << value;
                return QVariant{};
            }

            auto object = value.toObject();
            auto *qobject = metaObject->newInstance<QObject *>(nullptr);

            if (qobject == nullptr) {
                qCCritical(self) << "failed to create class:" << metaType.name();
                return QVariant{};
            }

            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                auto property = metaObject->property(i);

                if (!property.isWritable())
                    continue;

                auto propertyType = property.userType();
                auto propertyValue = object[property.name()];

                if (!property.write(qobject, this->deserialize(propertyType, propertyValue))) {
                    qCCritical(self) << "failed to write" << propertyValue << "to" << metaType.name() << property.name();
                }
            }

            return QVariant::fromValue(qobject);
        };
    }

    if (metaType.flags().testFlag(QMetaType::IsGadget)) {
        return [this, metaObject, typeId](const QJsonValue &value) {
            static QMetaType metaType(typeId);

            void *gadget = metaType.create();
            auto object = value.toObject();

            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                auto property = metaObject->property(i);
                auto propertyType = property.userType();
                auto propertyValue = object[property.name()];
                property.writeOnGadget(gadget, this->deserialize(propertyType, propertyValue));
            }

            return QVariant(metaType, const_cast<void *>(gadget));
        };
    }

    qFatal("Failed to build deserializer!");
}
