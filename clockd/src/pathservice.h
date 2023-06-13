#ifndef PATHSERVICE_H
#define PATHSERVICE_H

#include <QMap>
#include <QObject>
#include <QStandardPaths>

class PathService
{
public:
    static QString AppHome;
    static QString create(const QString &filename, bool file = true);
    static void init();

private:
};

#endif // PATHSERVICE_H
