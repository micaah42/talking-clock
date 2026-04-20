#ifndef LINKPREVIEWSERVICE_H
#define LINKPREVIEWSERVICE_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>

#include "linkpreview.h"

class LinkPreviewService : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit LinkPreviewService(QObject *parent = nullptr);

signals:

public slots:
    LinkPreview *getLinkPreview(const QUrl &url, QObject *parent = nullptr);

private:
    QNetworkAccessManager _manager;
};

#endif // LINKPREVIEWSERVICE_H
