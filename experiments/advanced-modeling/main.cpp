#include <QGuiApplication>
#include <QMetaProperty>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "container.h"
#include "remoting.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Container b;
    b.model()->push(Serializable{2112, "is awesome!"});
    b.model()->push(Serializable{2112, "is awesome!"});
    b.model()->set(1, Serializable{2112, "is wery awesome!"});
    // qRegisterMetaType<SerializableModel>("SerializableModel");

    engine.rootContext()->setContextProperty("cModel", &b);

    const QUrl url("qrc:/AdMoMain.qml");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qWarning() << "loading went wrong!";
                QCoreApplication::exit(-2);
            }
        },
        Qt::QueuedConnection);

    qInfo() << "loading view...";
    engine.load(url);
    qInfo() << "done!";

    qInfo() << "starting remoting...";
    Remoting remoting;
    remoting.registerObject("remoting", &remoting);
    remoting.registerObject("container", &b);

    return app.exec();
}
