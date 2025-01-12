#include "fontservice.h"

#include <QDir>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "setting.h"

namespace {
Q_LOGGING_CATEGORY(self, "fonts")
}

FontService::FontService(QQmlApplicationEngine *parent)
    : QObject(parent)
    , _fontDirectories(QList<QDir>{QDir(":fonts")})
    , _engine(parent)
    , _settings{DEFAULT_SETTINGS_SCOPE}
    , _family{"Font/Family", "Working Man"}
{
    this->refreshFamilies();
    _families.setSourceModel(&__families);
    QGuiApplication::setFont(QFont{_family});
}

void FontService::refreshFamilies()
{
    int loaded = 0;
    qCInfo(self) << "loading fonts...";
    for (auto const &fonts : std::as_const(_fontDirectories)) {
        if (!fonts.exists()) {
            qCCritical(self) << "fonts directory does not exist:" << fonts.path();
        }

        for (auto const &info : fonts.entryInfoList()) {
            if (info.suffix() != "ttf") {
                continue;
            }

            int id = QFontDatabase::addApplicationFont(info.filePath());

            if (id == -1)
                qCWarning(self) << "failed to load:" << info.filePath();
            else
                loaded++;
        }
    }

    qCInfo(self) << "fonts loaded:" << loaded;
    __families.setStringList(_dataBase.families());
    emit familiesChanged();

    auto current = QGuiApplication::font();
    qCInfo(self) << "current family:" << current.family();
}

QSortFilterProxyModel *FontService::families()
{
    return &_families;
}

QString FontService::family() const
{
    return _family;
}

void FontService::setFamily(const QString &family)
{
    qCInfo(self) << "set font:" << family;
    QGuiApplication::setFont(QFont{family});
    _family = family;
    emit familyChanged();

    if (!_engine) {
        qCCritical(self) << "engine not set! cannot reload qml!";
        return;
    }

    if (_engine->rootObjects().isEmpty()) {
        qCInfo(self) << "engine already about to load";
        return;
    }
}
