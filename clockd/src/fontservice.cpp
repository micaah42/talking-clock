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
    : QObject(parent), _currentFamilyIndex(0), _fontDirectories(QList<QDir>{QDir(":fonts")}),
      _engine(parent), _settingsService(settingsService)
{
    Q_ASSERT(settingsService);
    loadFonts();

    _settingsService->create("fontfamily", "Working Man", families());
    _settingsService->registerCallback("fontfamily", [this](const QVariant &value) {
        setFont(value.toString());
    });
}

void FontService::loadFonts()
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
    if (_dataBase.families() == _families) {
        return;
    }
    _families = _dataBase.families();
    emit familiesChanged();

    auto current = QGuiApplication::font();
    qDebug() << current.family();
    _currentFamilyIndex = _families.indexOf(current.family());
    emit currentFamiliyIndexChanged();
}

QVariantList FontService::families() const
{
    QVariantList list;
    for (auto const &family : _families) {
        list.append(family);
    }
    return list;
}

QString FontService::family() const
{
    return _families[_currentFamilyIndex];
}

int FontService::currentFamilyIndex() const
{
    return _currentFamilyIndex;
}

void FontService::setFont(const QString &family, const QString &style)
{
    qCInfo(self) << "set font:" << family << style;
    QGuiApplication::setFont(_dataBase.font(family, style, 0));
    _currentFamilyIndex = _families.indexOf(family);
    emit currentFamiliyIndexChanged();
    emit familyChanged();

    if (!_engine) {
        qCCritical(self) << "engine not set! cannot reload qml!";
        return;
    }

    if (_engine->rootObjects().isEmpty()) {
        qCInfo(self) << "engine already about to load";
        return;
    }

    //QQuickWindow *window = qobject_cast<QQuickWindow *>(_engine->rootObjects().at(0));
    //if (!window) {
    //    qCCritical(self) << "could not find QQuickWindow!";
    //    return;
    //}

#ifndef Q_PROCESSOR_ARM
//    qCInfo(self) << "load new window...";
//    _engine->load(QUrl("qrc:/Main.qml"));
//    qCInfo(self) << "close old window...";
//    //window->close();
#else
    qCWarning(self) << "font changes after reload!";
#endif
}
