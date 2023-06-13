#ifndef SETTINGSSERVICE_H
#define SETTINGSSERVICE_H

#include <QJsonArray>
#include <QObject>
#include <QSettings>
#include <QVariant>

struct Setting
{
    Q_GADGET
public:
    enum Input { FontDialog = 0, TextLine, Float, Integer };
    Q_ENUM(Input)

    QVariant value;
    QVariant::Type type;
    Input input;
};

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

typedef std::function<void(const QVariant &)> Callback;

class SettingsService : public QObject
{
    Q_OBJECT
public:
    explicit SettingsService(QObject *parent = nullptr);

    void registerCallback(const QString &key, Callback callback, const bool call = true);
    Q_INVOKABLE QVariant value(const QString &key) const;
    Q_INVOKABLE QStringList keys() const;
    QJsonArray settings() const;

signals:
    void valueChanged(const QString &key, const QVariant &value);
    void settingsChanged();

public slots:
    void create(const QString &key,
                const QVariant &value,
                const Setting::Input &input = Setting::TextLine);

    void setValue(const QString &key, QVariant value);

private:
    QSettings _settingsFile;
    QMap<QString, Setting> _settings;
    QMap<QString, QList<Callback>> _callbacks;

    Q_PROPERTY(QJsonArray settings READ settings NOTIFY settingsChanged)
};

#endif // SETTINGSSERVICE_H
