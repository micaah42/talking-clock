#ifndef PATHSERVICE_H
#define PATHSERVICE_H

#include <QDir>
#include <QObject>
#include <QQmlEngine>

class PathService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString shareDir READ shareDir CONSTANT FINAL)
    Q_PROPERTY(QString confDir READ confDir CONSTANT FINAL)

public:
    explicit PathService(QObject *parent = nullptr);

    static QString shareDir();
    static QString confDir();

public slots:
    static QString configFilePath(const QString &fileName);
    static QString shareFilePath(const QString &fileName);

    static bool writeFile(const QString &fileName, const QByteArray &data);
    static QByteArray readFile(const QString &fileName);
    static bool fileExists(const QString &fileName);

private:
    static QDir _shareDir;
    static QDir _confDir;
};

#endif // PATHSERVICE_H
