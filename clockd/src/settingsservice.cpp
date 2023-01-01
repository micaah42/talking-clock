#include "settingsservice.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QStandardPaths>

#include "jsonspecification.h"

namespace {
Q_LOGGING_CATEGORY(self, "settings")
}

SettingsService::SettingsService(QObject *parent)
    : QObject{parent}
{
    qCInfo(self) << "settings file:" << _settingsFile.fileName() << _settingsFile.format();
}

QVariant SettingsService::value(const QString &key) const
{
    if (!_settings.contains(key)) {
        qCCritical(self) << "invalid get access:" << key;
        return QVariant();
    }
    return _settings[key].value;
}

QStringList SettingsService::keys() const
{
    return _settings.keys();
}

void SettingsService::registerCallback(const QString &key, Callback callback, const bool call)
{
    Q_ASSERT(_settings.contains(key));
    _callbacks[key].append(callback);
    if (call) {
        callback(_settings[key].value);
    }
}

void SettingsService::create(const QString &key, const QVariant &value, const QVariantList &options)
{
    // Q_ASSERT(!_settings.contains(key));
    qCInfo(self) << "create:" << key << "->" << value << "options" << options.size();
    _settings[key] = {value, value.type(), options};

    if (_settingsFile.contains(key)) {
        qCInfo(self) << "loading from file ...";
        setValue(key, _settingsFile.value(key));
    }
    emit settingsChanged();
    emit valueChanged(key, _settings[key].value);
}

void SettingsService::setValue(const QString &key, QVariant value)
{
    if (!_settings.contains(key)) {
        qCCritical(self) << "invalid set access:" << key;
        return;
    }
    if (!value.convert(_settings[key].type)) {
        qCWarning(self) << "cannot convert" << value << "to" << _settings[key].type << "for" << key;
        return;
    }
    if (value == _settings[key].value) {
        return;
    }
    qCInfo(self) << "changed:" << key << _settings[key].value << "->" << value;
    _settings[key].value = value;
    _settingsFile.setValue(key, value);
    for (auto const &callback : _callbacks[key]) {
        callback(_settings[key].value);
    }
    emit valueChanged(key, _settings[key].value);
}

QJsonArray SettingsService::settings() const
{
    QJsonArray arr;
    for (auto const &setting : _settings.keys()) {
        QJsonArray opts;
        for (auto const &option : _settings[setting].options) {
            opts.append(option.toJsonValue());
        }
        arr.append(QJsonObject{{"key", setting},
                               {"value", _settings[setting].value.toJsonValue()},
                               {"type", static_cast<int>(_settings[setting].type)},
                               {"options", opts}});
    }
    return arr;
}
