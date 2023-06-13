#ifndef FONTSERVICE_H
#define FONTSERVICE_H

#include <QDir>
#include <QFontDatabase>
#include <QJsonArray>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QVariant>

class SettingsService;
class QQmlApplicationEngine;

class FontService : public QObject
{
    Q_OBJECT
public:
    explicit FontService(SettingsService *settingsService, QQmlApplicationEngine *parent = nullptr);

    Q_INVOKABLE void refreshFamilies();
    QSortFilterProxyModel *families();

    const QString &family() const;
    void setFamily(const QString &family);

signals:
    void familyChanged();
    void familiesChanged();

private:
    // font loading & finding
    QFontDatabase _dataBase;
    QList<QDir> _fontDirectories;
    SettingsService *_settingsService;

    // list of available families
    QStringListModel __families;
    QSortFilterProxyModel _families;

    // for setting the font
    QString _family;
    QQmlApplicationEngine *_engine;

    // --- --- ---
    Q_PROPERTY(QString family READ family WRITE setFamily NOTIFY familyChanged)
    Q_PROPERTY(QSortFilterProxyModel *families READ families NOTIFY familiesChanged)
};

#endif // FONTSERVICE_H
