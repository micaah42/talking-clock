#include "dataservice.h"

#include <QDir>
#include <QLoggingCategory>

namespace {
Q_LOGGING_CATEGORY(self, "data")
}

DataService::DataService(QObject *parent)
    : QObject{parent}
{
    refresh();
}

void DataService::refresh()
{
    auto types = QDir{"/resources"}.entryInfoList();
    qCDebug(self) << types;
    for (auto const &type : types) {
        if (!type.isDir() || type.baseName() == "")
            continue;

        auto sourceType = type.baseName();
        auto names = QDir{type.filePath()}.entryInfoList();
        qCDebug(self) << names;

        for (auto const &name : names) {
            if (!name.isDir() || name.baseName() == "")
                continue;

            auto sourceName = name.baseName();
            auto fetches = QDir{name.filePath()}.entryInfoList({"fetch*.csv"});

            int size = 0;
            for (const auto &fetch : fetches)
                size += fetch.size();

            _datasets.push(Dataset{sourceName, sourceType, name.fileName(), 0, fetches.size()});
        }
    }
    qCInfo(self) << "found" << _datasets.size() << "datasets";
}

DatasetModel *DataService::datasets()
{
    return &_datasets;
}
