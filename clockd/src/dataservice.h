#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QObject>

#include "templatemodel.h"

struct Dataset
{
    Q_GADGET
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(int bytes READ bytes WRITE setBytes)
    Q_PROPERTY(int fetches READ fetches WRITE setFetches)
    Q_PROPERTY(QString path READ path WRITE setPath)

public:
    explicit Dataset(const QString &name = "",
                     const QString &type = "",
                     const QString &path = "",
                     const int bytes = 0,
                     const int fetches = 0)
        : _name{name}
        , _type{type}
        , _path{path}
        , _fetches{fetches}
        , _bytes{bytes} {};

    QString name() const { return _name; }
    void setName(const QString &newName) { _name = newName; }

    int bytes() const { return _bytes; }
    void setBytes(int newBytes) { _bytes = newBytes; }

    int fetches() const { return _fetches; }
    void setFetches(int newFetches) { _fetches = newFetches; }

    QString path() const { return _path; }
    void setPath(const QString &newPath) { _path = newPath; }

    QString type() const { return _type; }
    void setType(const QString &newType) { _type = newType; }

private:
    QString _name;
    QString _type;
    QString _path;
    int _fetches;
    int _bytes;
};

DECLARE_MODEL(Dataset)

class DataService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DatasetModel *datasets READ datasets CONSTANT)
public:
    explicit DataService(QObject *parent = nullptr);
    Q_INVOKABLE void refresh();
    DatasetModel *datasets();

signals:

private:
    DatasetModel _datasets;
};

#endif // DATASERVICE_H
