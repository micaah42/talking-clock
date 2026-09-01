#ifndef CLOCKPROMPTBUILDER_H
#define CLOCKPROMPTBUILDER_H

#include <QObject>
#include <QQmlEngine>

#include "actionday.h"
#include "alarm.h"

#include "enumutil.h"
#include "setting.h"

class PromptBuilder : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(EnumUtilBase *moodEnum READ moodEnum CONSTANT FINAL)
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
        DisappointedParent,
    };

    Q_ENUM(Mood);

    explicit PromptBuilder(QObject *parent = nullptr);

    EnumUtilBase *moodEnum() const;

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

private:
    EnumUtilBase *_moodEnum;
};

class PromptBuilderSettings : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(PromptBuilder::Mood mood READ mood WRITE setMood NOTIFY moodChanged FINAL)
    Q_PROPERTY(bool showPrompt READ showPrompt WRITE setShowPrompt NOTIFY showPromptChanged FINAL)
    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged FINAL)

public:
    explicit PromptBuilderSettings(QObject *parent = nullptr);

    PromptBuilder::Mood mood() const;
    void setMood(const PromptBuilder::Mood &newMood);

    bool showPrompt() const;
    void setShowPrompt(bool newShowPrompt);

    QString model() const;
    void setModel(const QString &newModel);

signals:
    void moodChanged();

    void showPromptChanged();

    void modelChanged();

private:
    Setting<PromptBuilder::Mood> _mood;
    Setting<bool> _showPrompt;
    QString _model;
};

#endif // CLOCKPROMPTBUILDER_H
