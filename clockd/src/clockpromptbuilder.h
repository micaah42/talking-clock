#ifndef CLOCKPROMPTBUILDER_H
#define CLOCKPROMPTBUILDER_H

#include <QObject>
#include <QQmlEngine>

#include "actionday.h"
#include "alarm.h"

class PromptBuilder : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    enum Mood {
        Enthusiastic,
        Knightly,
        Angry,
        Deranged,
        Stoned,
        Depressed,
        Alien,
        SlightlyUpset,
        HomicidalSpaceAI,
        Formal,
    };

    Q_ENUM(Mood);

    explicit PromptBuilder(QObject *parent = nullptr);

public slots:
    static QString create(
        const QList<Alarm *> &upcomingAlarms,
        const QList<Alarm *> &triggeredAlarms,
        const QList<ActionDay *> &actionDays,
        PromptBuilder::Mood mood,
        const QString &extra = QString()
    );

    static QString sanitizeOutput(const QString &input);
};

class PromptBuilderSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(PromptBuilder::Mood mood READ mood WRITE setMood NOTIFY moodChanged FINAL)

public:
    explicit PromptBuilderSettings(QObject *parent = nullptr);

    PromptBuilder::Mood mood() const;
    void setMood(const PromptBuilder::Mood &newMood);

signals:
    void moodChanged();

private:
    PromptBuilder::Mood _mood;
};

#endif // CLOCKPROMPTBUILDER_H
