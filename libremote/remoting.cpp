#include "remoting.h"

#include <QDateTime>
#include <QLoggingCategory>

#include "templatemodel.h"

namespace {
Q_LOGGING_CATEGORY(self, "remoting", QtWarningMsg);
}

QByteArray Remoting::boink()
{
    auto now = QDateTime::currentDateTime();
    QJsonObject obj = {{"boink", now.toMSecsSinceEpoch()}};
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

Remoting::Remoting(QObject *parent) : QObject{parent}
{
    //connect(&_server, &ClockServer::messageReceived, this, &Remoting::onMessageReceived);
}

void Remoting::registerObject(const QString &name, QObject *object)
{
    Q_ASSERT(!_objects.contains(name));
    _objects[name] = object;
    qCInfo(self) << "registering:" << name << object;

    if (object == nullptr)
        return;

    auto metaObject = object->metaObject();

    // register methods

    for (int i = metaObject->methodOffset(); i < metaObject->methodCount(); i++) {
        auto method = metaObject->method(i);
        if (method.access() != QMetaMethod::Public || method.methodType() != QMetaMethod::Method)
            continue;

        auto key = QString("%1.%2").arg(name, method.name());
        _methods[key] = {object, method};
        qCInfo(self) << "+   method:" << key << method.parameterNames() << method.typeName();
    }

    // we need a the notifier slot meta method for our connect signatures
    auto notifierSlotIdx = this->metaObject()->indexOfMethod("onNotifySignal()");
    auto notifierSlot = this->metaObject()->method(notifierSlotIdx);

    // register properties
    for (int i = 0; i < metaObject->propertyCount(); i++) {
        auto property = metaObject->property(i);
        auto key = QString("%1.%2").arg(name, property.name());

        auto notify = property.hasNotifySignal() ? "notify" : "";
        qCInfo(self) << "+ property:" << key << property.typeName() << property.name() << notify;

        _properties[key] = {object, property};

        if (property.hasNotifySignal()) {
            _notifierCache[{object, property.notifySignalIndex()}] = key;
            connect(object, property.notifySignal(), this, notifierSlot);
        }

        // recursively register if the property is itself an object or gadget
        auto propertyMetaObject = QMetaType(property.userType()).metaObject();
        if (propertyMetaObject) {
            qCInfo(self) << "  * recursing..." << property.typeName() << property.name();
            auto propertyObject = property.read(object).value<QObject *>();
            this->registerObject(QString("%1.%2").arg(name, property.name()), propertyObject);
        }
    }

    // and if this object is of type template model, send changes
    auto model = qobject_cast<QAbstractTableModel *>(object);
    if (model) {
        model->setObjectName(name);
        connect(model, &QAbstractTableModel::dataChanged, this, &Remoting::onModelChanged);
        connect(model, &QAbstractTableModel::rowsRemoved, this, &Remoting::onModelRowsRemoved);
    }
}

QJsonValue Remoting::value(const QString &key)
{
    if (!_properties.contains(key)) {
        qCWarning(self) << "no such property registered:" << key;
        throw std::runtime_error("invalid key");
    }

    auto prop = _properties[key];
    auto value = prop.second.read(prop.first);
    qCInfo(self) << "value:" << key << "->" << value;

    auto json = VariantSerializer::I()->serialize(value);
    if (json.isUndefined()) {
        qCWarning(self) << "conversion failed!";
        throw std::runtime_error("conversion failed");
    }

    return json;
}

void Remoting::subscribe(const QString &key) {}

QJsonArray Remoting::receivers()
{
    QJsonArray arr;
    for (auto it = _objects.keyBegin(); it != _objects.keyEnd(); ++it) {
        arr.append(*it);
    }
    return arr;
}

QStringList Remoting::methods()
{
    return _methods.keys();
}

QStringList Remoting::properties()
{
    return _properties.keys();
}

void Remoting::onNotifySignal()
{
    if (!_notifierCache.contains({sender(), senderSignalIndex()})) {
        qCCritical(self) << "could not find key of notifier signal";
        return;
    }

    auto key = _notifierCache[{sender(), senderSignalIndex()}];
    auto value = this->value(key);

    auto obj = QJsonObject{{"key", key}, {"value", value}};
    QJsonDocument doc{QJsonObject{{"notifications", QJsonArray{obj}}}};
    // _server.broadcastMessage(doc.toJson(QJsonDocument::Compact));
}

void Remoting::onModelChanged(const QModelIndex &from, const QModelIndex &to, QVector<int> roles)
{
    auto model = qobject_cast<QAbstractTableModel *>(sender());
    Q_ASSERT(model);

    QJsonArray notifications;
    for (int i = from.row(); i <= to.row(); ++i) {
        auto key = QString("%1#%3").arg(model->objectName(), QString::number(i, 16));
        QJsonObject values;
        for (const int role : qAsConst(roles)) {
            auto value = VariantSerializer::I()->serialize(model->data(model->index(i, 0), role));
            auto const roleNames = model->roleNames();
            values[roleNames[role]] = value;
            notifications.append(QJsonObject{{"value", values}, {"key", key}});
        }
    }

    QJsonDocument doc{QJsonObject{{"notifications", notifications}}};
    // _server.broadcastMessage(doc.toJson(QJsonDocument::Compact));
}

void Remoting::onModelRowsRemoved(const QModelIndex &parent, const int first, const int last)
{
    auto model = qobject_cast<QAbstractTableModel *>(sender());
    Q_ASSERT(model);

    QJsonArray notifications;
    for (int i = first; i <= last; ++i) {
        auto key = QString("%1#%3").arg(model->objectName(), QString::number(i, 16));
        notifications.append(QJsonObject{{"key", key}, {"value", QJsonValue::Null}});
    }

    QJsonDocument doc{QJsonObject{{"notifications", notifications}}};
    //   _server.broadcastMessage(doc.toJson(QJsonDocument::Compact));
}

void Remoting::onMessageReceived(const QByteArray &message, const QUuid &clientId)
{
    // parse message
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(message, &error);
    if (error.error != QJsonParseError::NoError) {
        qCCritical(self) << "failed to message:" << message << "error:" << error.errorString();
        return;
    }
    auto obj = doc.object();

    // answer ping
    if (obj.contains("ping")) {
        qCInfo(self) << "received ping:" << obj;
        //   _server.sendMessage(boink(), clientId);
    }
    // answer normal command
    else if (obj["receiver"].isString() && obj["method"].isString()) {
        auto answer = processCommand(obj);
        qCInfo(self) << "handled:" << obj << "->" << answer;
        //    _server.sendObjectMessage(answer, clientId);
    }
    // error
    else {
        qCCritical(self) << "received invalid object:" << obj;
    }
}

QJsonObject Remoting::processCommand(const QJsonObject &cmd)
{
    // get id
    int id = cmd["id"].toInt();

    // get QObject*
    // auto receiver = cmd["receiver"].toString();
    // if (!_objects.contains(receiver)) {
    //     qCCritical(self) << "no such receiver:" << receiver;
    //     return QJsonObject{{"id", id}, {"error", "invalid_receiver"}};
    // }
    // auto object = _objects[receiver];

    auto method = QString("%1.%2").arg(cmd["receiver"].toString(), cmd["method"].toString());

    if (!_methods.contains(method)) {
        qCCritical(self) << "no such method:" << method;
        return QJsonObject{{"id", id}, {"error", "invalid_method"}};
    }

    auto pair = _methods[method];
    auto object = pair.first;
    auto metaMethod = pair.second;

    // parse arguments
    auto args = cmd["args"].toArray();

    if (metaMethod.parameterCount() > args.size()) {
        qCCritical(self) << "too many parameters expected:" << metaMethod.parameterCount() << "got:" << args.size();
        return QJsonObject{{"id", id}, {"error", "invalid_args"}};
    }

    QVariantList variants;

    for (int i = 0; i < args.size(); i++) {
        auto variant = VariantSerializer::I()->deserialize(metaMethod.parameterType(i), args[i]);
        if (metaMethod.parameterType(i) == QMetaType::QVariant) {
            // do not try to convert to variant
            variants.append(variant);
            continue;
        }
        else if (!variant.canConvert(metaMethod.parameterType(i))) {
            // warn for tricky conversions
            auto given = variant.typeName();
            auto wanted = QMetaType::typeName(metaMethod.parameterType(i));
            qCWarning(self) << "cannot convert" << given << "to" << wanted;
        }
        else if (!variant.convert(metaMethod.parameterType(i))) {
            // if conversion failed, send error
            qCCritical(self) << "bad conversion:" << args[i] << "->" << variant;
            return QJsonObject{{"id", id}, {"error", "invalid_args"}};
        }
        else {
            variants.append(variant);
        }
    }

    QList<QGenericArgument> gArgs;
    for (auto const &variant : variants) {
        QGenericArgument gArg(QMetaType::typeName(variant.userType()),
                              const_cast<void *>(variant.constData()));
        gArgs.append(gArg);
    }

#if QT_VERSION_MAJOR == 5
    QVariant returnValue(metaMethod.returnType(), static_cast<void *>(nullptr));
#else
    QVariant returnValue(metaMethod.returnMetaType(), static_cast<void *>(nullptr));
#endif
    QGenericReturnArgument gReturn(metaMethod.typeName(),
                                   const_cast<void *>(returnValue.constData()));
    try {
        bool ok = metaMethod.invoke(object,
                                    Qt::DirectConnection,
                                    gReturn,
                                    gArgs.value(0),
                                    gArgs.value(1),
                                    gArgs.value(2),
                                    gArgs.value(3),
                                    gArgs.value(4),
                                    gArgs.value(5),
                                    gArgs.value(6),
                                    gArgs.value(7),
                                    gArgs.value(8),
                                    gArgs.value(9));
        if (!ok) {
            qCWarning(self) << "calling" << metaMethod.methodSignature() << "failed.";
            return QJsonObject{{"id", id}, {"error", "method_error_occurred"}};
        }

    } catch (const std::exception &error) {
        return QJsonObject{{"id", id}, {"error", QString(error.what())}};
    }

    if (metaMethod.returnType() == QMetaType::Void) {
        return QJsonObject{{"id", id}, {"value", QJsonValue::Undefined}};
    }

    auto value = VariantSerializer::I()->serialize(returnValue);
    if (value.isNull() && (!returnValue.isNull() || returnValue.isValid())) {
        qCCritical(self) << "failed to convert return value to json:" << returnValue;
        return QJsonObject{{"id", id}, {"error", "return_value_serialization"}};
    }

    return QJsonObject{{"id", id}, {"value", value}};
}
