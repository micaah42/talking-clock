#ifndef SETTING_H
#define SETTING_H

#include <QJsonArray>
#include <QObject>
#include <QSettings>
#include <QVariant>

#ifdef Q_PROCESSOR_X86_64
#define DEFAULT_SETTINGS_SCOPE QSettings::UserScope
#else
#define DEFAULT_SETTINGS_SCOPE QSettings::SystemScope
#endif

template<class T>
class Setting
{
public:
    explicit Setting(const QString &key, const T &defaultValue, QSettings &settings)
        : _key{key}
        , _settings{settings}
        , _t{settings.value(key, QVariant::fromValue(defaultValue)).template value<T>()} {};

    T get() const { return _t; };

    void set(const T &t)
    {
        if (t == _t)
            return;

        _settings.setValue(_key, QVariant::fromValue(t));
        _t = t;
    };

    void operator=(const T &t) { set(t); };
    operator T() const { return get(); };

private:
    QString _key;
    QSettings &_settings;
    T _t;
};

#endif // SETTING_H
