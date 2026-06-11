#ifndef SPIXTOOL_H
#define SPIXTOOL_H

#include <QObject>
#include <QQmlEngine>

#include <setting.h>

class SpixTool : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool botEnabled READ botEnabled WRITE setBotEnabled NOTIFY botEnabledChanged FINAL)
    Q_PROPERTY(bool serverEnabled READ serverEnabled WRITE setServerEnabled NOTIFY serverEnabledChanged FINAL)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY serverPortChanged FINAL)

public:
    explicit SpixTool(QObject *parent = nullptr);

    bool botEnabled() const;
    void setBotEnabled(bool newBotEnabled);

    bool serverEnabled() const;
    void setServerEnabled(bool newServerEnabled);

    int serverPort() const;
    void setServerPort(int newServerPort);

signals:
    void botEnabledChanged();
    void serverEnabledChanged();
    void serverPortChanged();

private:
    struct Private;
    Private *_private;

    Setting<bool> _botEnabled;
    Setting<bool> _serverEnabled;
    Setting<int> _serverPort;
};

#endif // SPIXTOOL_H
