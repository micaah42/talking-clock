#include "linkpreview.h"

LinkPreview::LinkPreview(const QUrl &url, QObject *parent)
    : QObject{parent}
    , _url{url}
    , _status{Unknown}
{}

LinkPreview::LinkPreviewStatus LinkPreview::status() const
{
    return _status;
}

void LinkPreview::setStatus(LinkPreviewStatus status)
{
    if (_status == status)
        return;

    _status = status;
    emit statusChanged();
}

QString LinkPreview::ogTitle() const
{
    return _ogTitle;
}

void LinkPreview::setOgTitle(const QString &title)
{
    if (_ogTitle == title)
        return;

    _ogTitle = title;
    emit ogTitleChanged();
}

QString LinkPreview::ogDescription() const
{
    return _ogDescription;
}

void LinkPreview::setOgDescription(const QString &description)
{
    if (_ogDescription == description)
        return;

    _ogDescription = description;
    emit ogDescriptionChanged();
}

QString LinkPreview::ogImage() const
{
    return _ogImage;
}

void LinkPreview::setOgImage(const QString &image)
{
    if (_ogImage == image)
        return;

    _ogImage = image;
    emit ogImageChanged();
}

QString LinkPreview::ogUrl() const
{
    return _ogUrl;
}

void LinkPreview::setOgUrl(const QString &url)
{
    if (_ogUrl == url)
        return;

    _ogUrl = url;
    emit ogUrlChanged();
}

QString LinkPreview::ogSiteName() const
{
    return _ogSiteName;
}

void LinkPreview::setOgSiteName(const QString &siteName)
{
    if (_ogSiteName == siteName)
        return;

    _ogSiteName = siteName;
    emit ogSiteNameChanged();
}

QUrl LinkPreview::url() const
{
    return _url;
}

void LinkPreview::setUrl(const QUrl &newUrl)
{
    if (_url == newUrl)
        return;
    _url = newUrl;
    emit urlChanged();
}
