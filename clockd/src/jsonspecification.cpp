#include "jsonspecification.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "json")
}

JsonSpec::JsonSpec(std::initializer_list<std::pair<QString, QJsonValue::Type>> specs)
{
    for (auto const &spec : specs) {
        set(spec.first, spec.second);
    }
}

void JsonSpec::set(const QString &key, const QJsonValue::Type type)
{
    _specs[key] = type;
}

bool JsonSpec::check(const QJsonObject &obj)
{
    for (auto it = _specs.keyBegin(); it != _specs.keyEnd(); it++) {
        if (obj.value(*it).type() != _specs[*it]) {
            qCWarning(self) << "spec failed. expected:" << _specs[*it]
                            << "got:" << obj.value(*it).type() << "for key:" << *it;
            return false;
        }
    }
    return true;
}
