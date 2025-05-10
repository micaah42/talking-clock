#ifndef MODEL_H
#define MODEL_H

#include <QMetaProperty>
#include <QObject>
#include <QRegularExpression>

#include "orm/db.hpp"

class ModelBase : public QObject
{
    Q_OBJECT
public:
    explicit ModelBase(QObject *parent = nullptr);
signals:
};

template<class Entity>
class Model : public ModelBase
{
public:
    using QueryBuilder = std::shared_ptr<Orm::QueryBuilder>;

    explicit Model(QObject *parent = nullptr);

    QList<Entity *> get(const std::shared_ptr<Orm::QueryBuilder> &queryBuilder, QObject *parent);
    std::shared_ptr<Orm::QueryBuilder> queryBuilder();

private:
    const QMetaObject *_metaObject;
    std::shared_ptr<Orm::QueryBuilder> _queryBuilder;

    QByteArrayList fromCamelCase(const QByteArrayList &s);
    QByteArray fromCamelCase(const QByteArray &s);
};

template<class Entity>
Model<Entity>::Model(QObject *parent)
    : ModelBase{parent}
    , _metaObject{QMetaType::fromType<Entity>().metaObject()}
{}

template<class Entity>
inline std::shared_ptr<Orm::QueryBuilder> Model<Entity>::queryBuilder()
{
    static const QByteArray tableName = fromCamelCase(_metaObject->className());
    return Orm::DB::table(tableName);
}

template<class Entity>
inline QList<Entity *> Model<Entity>::get(const std::shared_ptr<Orm::QueryBuilder> &queryBuilder, QObject *parent)
{
    auto query = queryBuilder->get();

    QList<Entity *> entities;
    entities.reserve(query.size());

    while (query.next()) {
        auto entity = new Entity(parent);

        for (int i = _metaObject->propertyOffset(); i < _metaObject->propertyCount(); i++) {
            auto property = _metaObject->property(i);
            auto value = query.value(property.name());

            if (!value.canConvert(property.metaType())) {
                qCritical() << "incompatible type" << value.metaType() << "of" << _metaObject->className() << property.name();
                qFatal("db error!");
            }

            if (!property.write(entity, value)) {
                qCritical() << "failed to write property!";
            }
        }

        entities.append(entity);
    }

    return entities;
}

template<class Entity>
inline QByteArrayList Model<Entity>::fromCamelCase(const QByteArrayList &s)
{
    QByteArrayList ret;
    for (const auto &ss : s)
        ret.append(fromCamelCase(s));
    return ret;
}

template<class Entity>
inline QByteArray Model<Entity>::fromCamelCase(const QByteArray &s)
{
    static QRegularExpression regExp1{"(.)([A-Z][a-z]+)"};
    static QRegularExpression regExp2{"([a-z0-9])([A-Z])"};

    QString result = s;
    result.replace(regExp1, "\\1_\\2");
    result.replace(regExp2, "\\1_\\2");

    return result.toLower().toUtf8();
}

#endif // MODEL_H
