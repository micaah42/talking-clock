#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <QJsonObject>
#include <QStringList>

class Payload
{
public:
    // enum stuff
    enum DisplayMode { Unknown, Major, Minor };
    static QString displayModeToString(const DisplayMode mode);
    static DisplayMode stringToDisplayMode(const QString &string);
    // Q_ENUMS(DisplayMode);

    // payload
    Payload(const QString &sound = QString(),
            const QString &program = QString(),
            const QStringList &args = QStringList());

    QJsonObject toJson() const;
    bool fromJson(const QJsonObject &obj);

    const QString &sound() const;
    const QString &program() const;
    const QStringList &args() const;

private:
    DisplayMode _mode;
    QString _sound;
    QString _program;
    QStringList _args;
};

#endif // PAYLOAD_H
