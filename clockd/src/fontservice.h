#ifndef FONTSERVICE_H
#define FONTSERVICE_H

#include <QDir>
#include <QFontDatabase>
#include <QJsonArray>
#include <QObject>
#include <QVariant>

class SettingsService;
class QQmlApplicationEngine;

class FontService : public QObject
{
    Q_OBJECT
public:
    explicit FontService(SettingsService *settingsService, QQmlApplicationEngine *parent = nullptr);

    Q_INVOKABLE void setFont(const QString &family, const QString &style = "Bold");
    int currentFamilyIndex() const;

    Q_INVOKABLE void loadFonts();
    QVariantList families() const;

    QString family() const;

signals:
    void familyChanged();
    void familiesChanged();
    void currentFamiliyIndexChanged();

private:
    QStringList _families;
    QFontDatabase _dataBase;
    int _currentFamilyIndex;
    QList<QDir> _fontDirectories;
    QQmlApplicationEngine *_engine;
    SettingsService *_settingsService;

    // --- --- ---
    Q_PROPERTY(QString family READ family NOTIFY familyChanged)
    Q_PROPERTY(QVariantList families READ families NOTIFY familiesChanged)
    Q_PROPERTY(int currentFamilyIndex READ currentFamilyIndex NOTIFY currentFamiliyIndexChanged)
};

#endif // FONTSERVICE_H
