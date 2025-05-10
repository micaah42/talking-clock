#ifndef SETTING-OLD_H
#define SETTING-OLD_H

#include <QJsonArray>
#include <QObject>
#include <QSettings>
#include <QVariant>

#include "unistd.h"

template<class T>
class SettingOld
{
public:
    explicit SettingOld(const QString &key, const T &defaultValue)
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
QSettings &SettingOld<T>::_settings()
{
    static QSettings settings{geteuid() == 0 ? QSettings::SystemScope : QSettings::UserScope};
    qInfo() << "init settings file:" << settings.fileName();
    return settings;
}

#endif // SETTING-OLD_H
