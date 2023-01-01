/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * model.h                                                               *
 *                                                                       *
 * Model implements a QMap that keeps a QJsonArray of QJsonObjects with  *
 * key and value                                                         *
 * - michael, 2022-03                                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef MODEL_H
#define MODEL_H

#include <QMap>
#include <QObject>
#include <QVariant>

class VariantModel : public QObject
{
public:
    VariantModel(const int typeId, QObject *parent = nullptr);

    Q_INVOKABLE void set(const int &id, const QVariant &value);
    Q_INVOKABLE void remove(const int &id);

signals:

private:
    int _typeId;
    QMap<int, QVariant> _values;
};

#endif // MODEL_H
