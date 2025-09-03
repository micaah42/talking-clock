#ifndef FONTSERVICE_H
#define FONTSERVICE_H

#include <QDir>
#include <QFontDatabase>
#include <QJsonArray>
#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QVariant>

#include "setting.h"

class QQmlApplicationEngine;

class FontService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QSortFilterProxyModel *families READ families NOTIFY familiesChanged)
    Q_PROPERTY(QString family READ family WRITE setFamily NOTIFY familyChanged)

public:
    explicit FontService(QQmlApplicationEngine *parent = nullptr);

    Q_INVOKABLE void refreshFamilies();
    QSortFilterProxyModel *families();

    QString family() const;
    void setFamily(const QString &family);

signals:
    void familyChanged();
    void familiesChanged();

private:
    // font loading & finding
    QFontDatabase _dataBase;
    QList<QDir> _fontDirectories;

    // list of available families
    QStringListModel __families;
    QSortFilterProxyModel _families;

    // for setting the font
    Setting<QString> _family;
};

#endif // FONTSERVICE_H
