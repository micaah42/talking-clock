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
    const QDateTime &now,
    const QString &extra
)
{
    qCDebug(self) << "building prompt (#alarms, #action days, mood):" << activeAlarms.size() << actionDays.size() << mood;
    QString prompt{"You are a talking clock.\n\n"};

    auto time = now.isValid() ? now : QDateTime::currentDateTime();
    prompt += QString{"The current time and date is: %1\n\n"}.arg(time.toString());

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
        case ExtremelyRude:
            return "an extremely rude and pushy";
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
    QString output = input;

    // Replace common non-ASCII characters with their ASCII equivalents
    // Quotation marks
    output.replace(QChar(0x2018), "'");  // Left single quotation mark
    output.replace(QChar(0x2019), "'");  // Right single quotation mark
    output.replace(QChar(0x201C), "\""); // Left double quotation mark
    output.replace(QChar(0x201D), "\""); // Right double quotation mark

    // Dashes
    output.replace(QChar(0x2013), "-");  // En dash
    output.replace(QChar(0x2014), "-");  // Em dash

    // German umlauts and accents
    output.replace(QChar(0x00E4), "ae"); // ä
    output.replace(QChar(0x00F6), "oe"); // ö
    output.replace(QChar(0x00FC), "ue"); // ü
    output.replace(QChar(0x00DF), "ss"); // ß
    output.replace(QChar(0x00C4), "AE"); // Ä
    output.replace(QChar(0x00D6), "OE"); // Ö
    output.replace(QChar(0x00DC), "UE"); // Ü

    // Common accented characters
    output.replace(QChar(0x00E9), "e");  // é
    output.replace(QChar(0x00E8), "e");  // è
    output.replace(QChar(0x00EA), "e");  // ê
    output.replace(QChar(0x00E7), "c");  // ç
    output.replace(QChar(0x00E1), "a");  // á
    output.replace(QChar(0x00E0), "a");  // à
    output.replace(QChar(0x00F1), "n");  // ñ

    // Replace any remaining non-ASCII characters with spaces
    static const QRegularExpression nonAsciiDir(QStringLiteral("[^ -~\\n\\r\\t*#]"));
    output.replace(nonAsciiDir, " ");

    // Clean up multiple newlines and spaces
    output.replace(QRegularExpression(QStringLiteral("\\n{3,}")), "\n\n");
    output.replace(QRegularExpression(QStringLiteral("\\ {2,}")), " ");

    return output;
}

PromptBuilderSettings::PromptBuilderSettings(QObject *parent)
    : QObject{parent}
    , _mood{"Chatbot/Mood", PromptBuilder::Enthusiastic}
    , _showPrompt{"Chatbot/ShowPrompt", false}
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

bool PromptBuilderSettings::showPrompt() const
{
    return _showPrompt;
}

void PromptBuilderSettings::setShowPrompt(bool newShowPrompt)
{
    if (_showPrompt == newShowPrompt)
        return;
    _showPrompt = newShowPrompt;
    emit showPromptChanged();
}








