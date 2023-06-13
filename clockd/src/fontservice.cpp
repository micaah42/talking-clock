#include "fontservice.h"

#include <QDir>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QTimer>

#include "settingsservice.h"

namespace {
Q_LOGGING_CATEGORY(self, "fonts")
}

FontService::FontService(SettingsService *settingsService, QQmlApplicationEngine *parent)
    : QObject(parent)
    , _fontDirectories(QList<QDir>{QDir(":fonts")})
    , _engine(parent)
    , _settingsService(settingsService)
{
    Q_ASSERT(settingsService);
    refreshFamilies();
    _families.setSourceModel(&__families);

    _settingsService->create("fontfamily", "Working Man", Setting::FontDialog);
    _settingsService->registerCallback("fontfamily", [this](const QVariant &value) {
        setFamily(value.toString());
    });
}

void FontService::refreshFamilies()
{
    int loaded = 0;
    qCInfo(self) << "loading fonts...";
    for (auto const &fonts : qAsConst(_fontDirectories)) {
        if (!fonts.exists()) {
            qCCritical(self) << "fonts directory does not exist:" << fonts.path();
        }

        for (auto const &info : fonts.entryInfoList()) {
            if (info.suffix() != "ttf") {
                continue;
            }
            int id = QFontDatabase::addApplicationFont(info.filePath());
            if (id != -1) {
                loaded++;
            }
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

const QString &FontService::family() const
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
