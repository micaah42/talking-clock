#ifndef CLOCKPROMPTBUILDER_H
#define CLOCKPROMPTBUILDER_H

#include <QObject>
#include <QQmlEngine>

#include "actionday.h"
#include "alarm.h"
#include "chatbotresponseservice.h"

#include "chatbotresponse.h"
#include "listmodel.h"
#include "setting.h"

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
        ExtremelyRude,
    };

    Q_ENUM(Mood);

    explicit PromptBuilder(QObject *parent = nullptr);

public slots:
    static QString create(
        const QList<Alarm *> &upcomingAlarms,
        const QList<Alarm *> &triggeredAlarms,
        const QList<ActionDay *> &actionDays,
        PromptBuilder::Mood mood,
        const QDateTime &now = QDateTime(),
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
    Q_PROPERTY(bool showPrompt READ showPrompt WRITE setShowPrompt NOTIFY showPromptChanged FINAL)

public:
    explicit PromptBuilderSettings(QObject *parent = nullptr);

    PromptBuilder::Mood mood() const;
    void setMood(const PromptBuilder::Mood &newMood);

    bool showPrompt() const;
    void setShowPrompt(bool newShowPrompt);

signals:
    void moodChanged();

    void showPromptChanged();

private:
    Setting<PromptBuilder::Mood> _mood;
    Setting<bool> _showPrompt;
};

#endif // CLOCKPROMPTBUILDER_H
