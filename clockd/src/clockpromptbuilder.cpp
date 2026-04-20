#include "clockpromptbuilder.h"

#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "promptbuilder")
}

PromptBuilder::PromptBuilder(QObject *parent)
    : QObject{parent}
{}

QString alarmsStringFromList(const QList<Alarm *> &alarms)
{
    QStringList alarmStrings;
    for (const auto alarm : alarms)
        alarmStrings.append("\"" + alarm->name() + "\", scheduled for " + alarm->nextTimeout().toString());

    return alarmStrings.join(", ");
}

QString PromptBuilder::create(
    const QList<Alarm *> &upcomingAlarms,
    const QList<Alarm *> &activeAlarms,
    const QList<ActionDay *> &actionDays,
    Mood mood,
    const QString &extra
)
{
    qCDebug(self) << "building prompt (#alarms, #action days, mood):" << activeAlarms.size() << actionDays.size() << mood;
    QString prompt{"You are a talking clock.\n\n"};

    prompt += QString{"The current time and date is: %1\n\n"}.arg(QDateTime::currentDateTime().toString());

    if (!activeAlarms.empty()) {
        auto alarmsString = alarmsStringFromList(activeAlarms);
        auto alarmPrompt = QStringList{
            activeAlarms.size() == 1 ? "The alarm" : "The alarms",
            alarmsString,
            "has just been triggered.",
        }.join(' ');

        prompt += alarmPrompt + "\n\n";
    }

    if (!upcomingAlarms.empty()) {
        auto alarmsString = alarmsStringFromList(activeAlarms);
        auto alarmPrompt = QStringList{
            upcomingAlarms.size() == 1 ? "The alarm" : "The alarms",
            alarmsString,
            "is the next alarm to be triggerd.",
        }.join(' ');

        prompt += alarmPrompt + "\n\n";
    }

    if (!actionDays.empty()) {
        QStringList actionDayStrings;
        for (const auto actionDay : actionDays)
            actionDayStrings.append("* " + actionDay->name() + ": " + actionDay->desc());

        QString actionDayPrompt = QStringList{
            "The following international action",
            actionDays.size() == 1 ?  "day takes" : "days take",
            "place today, you may reference one ore two of them:\n",
        }.join(' ');

        actionDayPrompt += actionDayStrings.join("\n");

        prompt += actionDayPrompt + "\n\n";
    }

    auto moodModifier = [&]() {
        switch (mood) {
        case Enthusiastic:
            return "an enthusiastic";
        case Knightly:
            return "an mediaval knight's";
        case Angry:
            return "an angry";
        case Deranged:
            return "a deranged psychopath";
        case Stoned:
            return "a stoned hippie";
        case Depressed:
            return "a depressed robot";
        case Alien:
            return "an alien";
        case SlightlyUpset:
            return "a slightly upset";
        case HomicidalSpaceAI:
            return "a homicidal space AI";
        case Formal:
        default:
            return "a formal and professional";
        }
    }();

    if (!extra.isEmpty())
        prompt += extra;

    QString command = QString{"Write a message in natural language as if you were %1 assistant. Avoid emojis and special signs."}.arg(
        moodModifier
    );

    prompt += command;

    qCInfo(self) << "built prompt:" << prompt;
    return prompt;
}

QString PromptBuilder::sanitizeOutput(const QString &input)
{
    static const QRegularExpression nonAsciiDir(QStringLiteral("[^ -~\\n\\r\\t*#]"));

    QString output = input;
    output.replace(nonAsciiDir, " ");
    output.replace(QRegularExpression(QStringLiteral("\\n{3,}")), "\n\n");
    output.replace(QRegularExpression(QStringLiteral("\\ {2,}")), " ");
    return output;
}

PromptBuilderSettings::PromptBuilderSettings(QObject *parent)
    : QObject{parent}
    , _mood{PromptBuilder::Depressed}
{}

PromptBuilder::Mood PromptBuilderSettings::mood() const

{
    return _mood;
}

void PromptBuilderSettings::setMood(const PromptBuilder::Mood &newMood)
{
    if (_mood == newMood)
        return;

    _mood = newMood;
    emit moodChanged();
}
