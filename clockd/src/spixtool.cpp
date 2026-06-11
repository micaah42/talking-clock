#include "spixtool.h"

#include <QCoreApplication>

#define TC_REMOTE_TESTING 0

#if TC_REMOTE_TESTING

#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>

struct SpixTool::Private
{
    spix::AnyRpcServer *server;
    spix::QtQmlBot *bot;
};

#else

struct SpixTool::Private
{
    void *server;
    void *bot;
};
#endif

SpixTool::SpixTool(QObject *parent)
    : QObject{parent}
    , _private{new Private{nullptr, nullptr}}
    , _botEnabled{"SpixTool/BotEnabled", false}
    , _serverEnabled{"SpixTool/ServerEnabled", false}
    , _serverPort{"SpixTool/ServerPort", false}
{
    if (_serverEnabled) {
        this->setServerEnabled(false);
        this->setServerEnabled(true);
    }

    if (_botEnabled) {
        this->setBotEnabled(false);
        this->setBotEnabled(true);
    }
}

bool SpixTool::botEnabled() const
{
    return _botEnabled;
}

void SpixTool::setBotEnabled(bool newBotEnabled)
{
#if TC_REMOTE_TESTING
    if (_botEnabled == newBotEnabled)
        return;

    _botEnabled = newBotEnabled;
    emit botEnabledChanged();

    if (_botEnabled) {
        if (!_serverEnabled)
            this->setServerEnabled(true);

        _private->bot = new spix::QtQmlBot(this);
        _private->bot->runTestServer(*_private->server);
    } else {
        _private->bot->deleteLater();
        _private->bot = nullptr;
    }
#else
    return;
#endif
}

bool SpixTool::serverEnabled() const
{
#if TC_REMOTE_TESTING
    return _serverPort;
#else
    return false;
#endif
}

void SpixTool::setServerEnabled(bool newServerEnabled)
{
#if TC_REMOTE_TESTING
    if (_serverEnabled == newServerEnabled)
        return;

    _serverEnabled = newServerEnabled;
    emit serverEnabledChanged();

    if (_serverEnabled)
        _private->server = new spix::AnyRpcServer(_serverPort);
    else {
        if (_botEnabled)
            this->setBotEnabled(false);

        QMetaObject::invokeMethod(QCoreApplication::instance(), [server = _private->server]() { delete server; }, Qt::QueuedConnection);
        _private->server = nullptr;
    }
#else
    return;
#endif
}

int SpixTool::serverPort() const
{
#if TC_REMOTE_TESTING
    return _serverPort;
#else
    return false;
#endif
}

void SpixTool::setServerPort(int newServerPort)
{
    if (_serverPort == newServerPort)
        return;

    _serverPort = newServerPort;
    emit serverPortChanged();
}
