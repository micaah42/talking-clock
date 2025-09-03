#ifndef ABOUT_H
#define ABOUT_H

#include <QDir>
#include <QObject>
#include <QQmlEngine>
#include <QSortFilterProxyModel>
#include <QStringListModel>

class About : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QSortFilterProxyModel *packages READ packages CONSTANT FINAL)
public:
    explicit About(QObject *parent = nullptr);
    QSortFilterProxyModel *packages();

public slots:
    QStringList licenseText(const QString &package);

private:
    QStringListModel _packages;
    QSortFilterProxyModel _filterable;
    QDir _packagesDir;
};

#endif // ABOUT_H
