#ifndef LINKPREVIEW_H
#define LINKPREVIEW_H

#include <QObject>
#include <QQmlEngine>

class LinkPreview : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(LinkPreviewStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString ogTitle READ ogTitle NOTIFY ogTitleChanged)
    Q_PROPERTY(QString ogDescription READ ogDescription NOTIFY ogDescriptionChanged)
    Q_PROPERTY(QString ogImage READ ogImage NOTIFY ogImageChanged)
    Q_PROPERTY(QString ogUrl READ ogUrl NOTIFY ogUrlChanged)
    Q_PROPERTY(QString ogSiteName READ ogSiteName NOTIFY ogSiteNameChanged)

public:
    enum LinkPreviewStatus {
        Unknown,
        Loading,
        Success,
        Error,
    };

    Q_ENUM(LinkPreviewStatus)

    LinkPreviewStatus status() const;
    QString ogTitle() const;
    QString ogDescription() const;
    QString ogImage() const;
    QString ogUrl() const;
    QString ogSiteName() const;
    QUrl url() const;

signals:
    void statusChanged();
    void ogTitleChanged();
    void ogDescriptionChanged();
    void ogImageChanged();
    void ogUrlChanged();
    void ogSiteNameChanged();
    void urlChanged();

protected:
    explicit LinkPreview(const QUrl &url, QObject *parent = nullptr);
    void setUrl(const QUrl &newUrl);
    void setStatus(LinkPreviewStatus status);
    void setOgTitle(const QString &title);
    void setOgDescription(const QString &description);
    void setOgImage(const QString &image);
    void setOgUrl(const QString &url);
    void setOgSiteName(const QString &siteName);

    friend class LinkPreviewService;

private:
    LinkPreviewStatus _status;
    QUrl _url;
    QString _ogTitle;
    QString _ogDescription;
    QString _ogImage;
    QString _ogUrl;
    QString _ogSiteName;
};

#endif // LINKPREVIEW_H
