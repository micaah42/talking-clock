#include "linkpreviewservice.h"

#include <QLoggingCategory>
#include <QNetworkReply>
#include <QRegularExpression>

namespace {
Q_LOGGING_CATEGORY(self, "linkpreviewservice")
}

LinkPreviewService::LinkPreviewService(QObject *parent)
    : QObject{parent}
{}

LinkPreview *LinkPreviewService::getLinkPreview(const QUrl &url, QObject *parent)
{
    LinkPreview *preview = new LinkPreview(url, parent);
    preview->setStatus(LinkPreview::Loading);

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::UserAgentHeader, "talking-clock/1.0 (github.com/micaah42)");

    qCDebug(self) << "requesting page:" << request.url();
    QNetworkReply *reply = _manager.get(request);

    connect(reply, &QNetworkReply::finished, preview, [this, reply, preview]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(self) << "network error:" << reply->errorString();
            preview->setStatus(LinkPreview::Error);
            return;
        }

        QString html = QString::fromUtf8(reply->readAll());

        auto extractMeta = [&](const QString &property) -> QString {
            // Matches <meta property="og:title" content="Some Title"> or reversed
            // Handles both ' and " quotes
            QString pattern = QString(R"(<meta\s+[^>]*property=["']og:%1["'][^>]*content=["']([^"']+)["'])").arg(property);
            QRegularExpression re(pattern, QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = re.match(html);

            // If not found by property, try 'name' attribute (common fallback)
            if (!match.hasMatch()) {
                pattern = QString(R"(<meta\s+[^>]*name=["']og:%1["'][^>]*content=["']([^"']+)["'])").arg(property);
                re.setPattern(pattern);
                match = re.match(html);
            }
            auto value = match.hasMatch() ? match.captured(1).trimmed() : QString();
            qCDebug(self) << "found metadata:" << property << value;
            return value;
        };

        // Populate properties

        preview->setOgTitle(extractMeta("title"));
        preview->setOgDescription(extractMeta("description"));
        preview->setOgImage(extractMeta("image"));
        preview->setOgUrl(extractMeta("url"));
        preview->setOgSiteName(extractMeta("site_name"));

        // Basic validation: if we didn't even get a title, we might consider it a failure
        if (preview->ogTitle().isEmpty() && preview->ogDescription().isEmpty()) {
            preview->setStatus(LinkPreview::Error);
        } else {
            preview->setStatus(LinkPreview::Success);
        }
    });

    return preview;
}
