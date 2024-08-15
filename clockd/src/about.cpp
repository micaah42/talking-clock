#include "about.h"

#include <QFile>
#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "about")
}

About::About(QObject *parent)
    : QObject{parent}
    , _packagesDir{"/usr/share/licenses"}
{
    auto packages = _packagesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    _packages.setStringList(packages);

    _filterable.setFilterCaseSensitivity(Qt::CaseInsensitive);
    _filterable.setSourceModel(&_packages);
}

QSortFilterProxyModel *About::packages()
{
    return &_filterable;
}

QStringList About::licenseText(const QString &package)
{
    QDir packageDir{_packagesDir.filePath(package)};
    auto files = packageDir.entryList(QDir::Files, QDir::Name);

    QStringList s;

    for (const auto &fileName : qAsConst(files)) {
        QFile file{packageDir.filePath(fileName)};

        if (!file.open(QIODevice::ReadOnly)) {
            qCCritical(self) << "failed to open" << file.fileName();
            continue;
        }

        while (true) {
            s.append(file.readLine());

            if (s.last().isEmpty())
                break;
        }

        s.append("");
    }

    return s;
}
