#ifndef MAPADAPTER_H
#define MAPADAPTER_H

#include <QAbstractListModel>
#include <QMetaProperty>
#include <QObject>

#include "libremote_global.h"
#include "serializer.h"

// to store a list of gadgets or objects
class LIBREMOTE_EXPORT TemplateModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TemplateModel(const QMetaType &metaType = QMetaType(), QObject *parent = nullptr);

    int classId() const { return _classId; };
    Q_INVOKABLE int size() const { return _items.size(); };
    QVariant item(int i) { return _items.at(i); };

    // table model functions
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

public slots:
    // void append();

protected:
    // c++ access
    template<class T>
    const T _at(const int index) const;

    template<class T>
    void _set(const int index, const T &value);

    template<class T>
    void _push(const T &value);

    template<class T>
    bool _contains(const T &value);

private:
    int _classId;
    QString _className;
    QList<QMetaProperty> _columns;

    QList<QVariant> _items;
};

template<class T>
bool TemplateModel::_contains(const T &value)
{
    return _items.contains(QVariant::fromValue(value));
}

template<class T>
const T TemplateModel::_at(const int index) const
{
    return _items[index].value<T>();
}

template<class T>
void TemplateModel::_set(const int index, const T &value)
{
    // get changed roles
    QVector<int> roles;
    for (auto const &property : qAsConst(_columns)) {
        auto a = property.readOnGadget(_items[index].data());
        auto b = property.readOnGadget(&value);
        if (a != b) {
            roles.append(property.propertyIndex() + Qt::UserRole);
        }
    }

    if (roles.isEmpty()) {
        return;
    }

    _items[index] = QVariant::fromValue(value);
    auto mindex = QAbstractItemModel::createIndex(index, 0);
    emit dataChanged(mindex, mindex, roles);
}

template<class T>
void TemplateModel::_push(const T &value)
{
    insertRows(_items.size(), 1);
    _set<T>(_items.size() - 1, value);
}

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

#define DECLARE_MODEL(C) \
    class C##Model : public TemplateModel \
    { \
        Q_OBJECT \
    public: \
        explicit C##Model(QObject *parent = nullptr) \
            : TemplateModel(QMetaType::fromType<C>(), parent){}; \
        Q_INVOKABLE const C at(const int index) const \
        { \
            return TemplateModel::_at<C>(index); \
        }; \
        Q_INVOKABLE void set(const int index, const C &value) \
        { \
            TemplateModel::_set<C>(index, value); \
        }; \
        Q_INVOKABLE void remove(const int i) \
        { \
            TemplateModel::removeRows(i, 1); \
        }; \
        Q_INVOKABLE void push(const C &value = C()) \
        { \
            TemplateModel::_push<C>(value); \
        }; \
        Q_INVOKABLE bool contains(const C &value) \
        { \
            return TemplateModel::_contains<C>(value); \
        }; \
    };

#endif // MAPADAPTER_H
