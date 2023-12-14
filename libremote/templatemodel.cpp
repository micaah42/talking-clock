#include "templatemodel.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "templatemodel");
}

TemplateModel::TemplateModel(const QMetaType &type, QObject *parent) : QAbstractTableModel(parent)
{
    if (!type.isRegistered())
        qFatal("unregistered type in template model %s", type.name());

    if (!type.isValid())
        qFatal("invalid type in template model %s", type.name());

    _classId = type.id();

    auto metaObject = type.metaObject();
    _className = metaObject->className();

    for (int i = 0; i < metaObject->propertyCount(); ++i)
        _columns.append(metaObject->property(i));
}

int TemplateModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _items.size();
}

int TemplateModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _columns.size();
}

QVariant TemplateModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _items.size() || index.row() < 0)
        return QVariant();

    auto gadget = _items[index.row()].data();
    return _columns[role - Qt::UserRole].readOnGadget(gadget);
}

QVariant TemplateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return QString("%1 [%2]").arg(_columns[section].name(), _columns[section].typeName());
    }

    return QVariant();
}

QHash<int, QByteArray> TemplateModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    for (int i = 0; i < _columns.size(); ++i)
        roles[i + Qt::UserRole] = _columns[i].name();
    return roles;
}

bool TemplateModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    QMetaType metaType(_classId);
    auto copy = metaType.create();
    for (int row = 0; row < rows; ++row) {
#if QT_VERSION_MAJOR == 5
        _items.insert(position, QVariant(_classId, copy));
#elif QT_VERSION_MAJOR == 6
        _items.insert(position, QVariant(metaType, copy));
#endif
    }

    endInsertRows();
    return true;
}

bool TemplateModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        _items.removeAt(position);

    endRemoveRows();
    return true;
}

bool TemplateModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(self) << "index invalid:" << index;
        return false;
    }
    if (index.row() >= _items.size() || index.column() != 0) {
        qCWarning(self) << "index invalid:" << index;
        return false;
    }

    auto oldValue = _items[index.row()];
    auto newValue = QVariant(value);
    if (!newValue.convert(_columns[role - Qt::UserRole].type())) {
        qCWarning(self) << "bad convert:" << value << _columns[role - Qt::UserRole].typeName();
        return false;
    }
    if (!_columns[role - Qt::UserRole].writeOnGadget(_items[index.row()].data(), value)) {
        qCWarning(self) << "failed to write:" << value << oldValue;
        return false;
    }
    if (value != oldValue) {
        qCInfo(self) << "set:" << index.row() << _columns[role - Qt::UserRole].name() << value;
        emit dataChanged(index, index, {role});
    }
    return true;
}

Qt::ItemFlags TemplateModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
