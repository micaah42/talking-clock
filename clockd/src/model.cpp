#include "model.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "variantmodel");
}

VariantModel::VariantModel(const int typeId, QObject *parent) : QObject{parent}, _typeId{typeId} {}

void VariantModel::set(const int &id, const QVariant &value)
{
    // value null => remove id
    if (value.isNull()) {
        if (_values.remove(id) == 0) {
            qCWarning(self) << "tried to remove non-existent item:" << id;
        }
    }
    // cannot convert warning
    else if (!value.canConvert(_typeId)) {
        qCWarning(self) << "cannot set:" << value.typeName() << "->" << QMetaType(_typeId).name();
    }
    // convert and sort in
    else {
        QVariant copy(value);
        copy.convert(_typeId);
        _values[id] = copy;
    }
}
