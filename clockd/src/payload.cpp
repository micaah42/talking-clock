#include "payload.h"

#include <QDebug>
#include <QJsonArray>

#include "jsonspecification.h"

QString Payload::displayModeToString(const DisplayMode mode)
{
    switch (mode) {
    case Major:
        return "major";
    case Minor:
        return "minor";
    default:
        return "unknown";
    }
}

Payload::DisplayMode Payload::stringToDisplayMode(const QString &string)
{
    if (string == "major")
        return Major;
    else if (string == "minor")
        return Minor;
    else {
        qInfo() << "Cannot convert %s to Payload::DisplayMode", qUtf8Printable(string);
        return Unknown;
    }
}

Payload::Payload(const QString &sound, const QString &program, const QStringList &args)
    : _sound(sound), _program(program), _args(args)
{}

QJsonObject Payload::toJson() const
{
    QJsonObject obj;
    obj["mode"] = displayModeToString(_mode);
    obj["sound"] = _sound;
    obj["program"] = _program;

    QJsonArray arr;
    for (auto const &arg : _args) {
        arr.append(arg);
    }
    obj["args"] = arr;
    return obj;
}

bool Payload::fromJson(const QJsonObject &obj)
{
    JsonSpec spec{
        {"mode", QJsonValue::String},
        {"sound", QJsonValue::String},
        {"program", QJsonValue::String},
        {"args", QJsonValue::Array},
    };
    if (!spec.check(obj)) {
        qWarning() << "failed to parse payload";
        return false;
    }
    _mode = stringToDisplayMode(obj["mode"].toString());
    _sound = obj["sound"].toString();
    _program = obj["program"].toString();

    QStringList args;
    for (auto const &arg : obj["args"].toArray()) {
        args.append(arg.toString());
    }
    _args = args;
    return true;
}

const QString &Payload::sound() const
{
    return _sound;
}

const QString &Payload::program() const
{
    return _program;
}

const QStringList &Payload::args() const
{
    return _args;
}
