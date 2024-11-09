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
    explicit Setting(const QString &key, const T &defaultValue)
        : _key{key} //
        , _t{_settings().value(key, QVariant::fromValue(defaultValue)).template value<T>()} {};

    T get() const { return _t; };

    void set(const T &t)
    {
        if (t == _t)
            return;

        _settings().setValue(_key, QVariant::fromValue(t));
        _t = t;
    };

    void operator=(const T &t) { set(t); };
    operator T() const { return get(); };

private:
    static QSettings &_settings();
    QString _key;
    T _t;
};

template<class T>
QSettings &Setting<T>::_settings()
{
    static QSettings settings{DEFAULT_SETTINGS_SCOPE};
    return settings;
}

#endif // SETTING_H
