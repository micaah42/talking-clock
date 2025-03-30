#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include "lighting.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("lighting-cli");
    QCoreApplication::setOrganizationName("micaah");
    QCoreApplication::setOrganizationDomain("org");
    QCoreApplication::setApplicationVersion("0.1");
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addOption({{"f", "frequency"}, "frequency in hz", "frequency", "0.1"});
    parser.addOption({{"l", "leds"}, "number of leds on the strip", "leds", "300"});
    parser.addOption({{"c", "color"}, "led color", "color", "white"});

    parser.addVersionOption();
    parser.addHelpOption();
    parser.parse(app.arguments());

    Lighting lighting{parser.value("leds").toInt()};
    lighting.setMode(lighting.staticLight());

    bool ok;

    QString colorString = parser.value("color");
    int colorValue = colorString.toInt(&ok, 16);

    if (ok && colorValue <= 0xffffff && colorString.size() == 6)
        colorString.prepend('#');

    lighting.staticLight()->setColor(colorString);

    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(75);
    timer.start();

    int startTime = QDateTime::currentMSecsSinceEpoch();
    double frequency = parser.value("frequency").toFloat();

    timer.callOnTimeout([&]() {
        double t = frequency * (QDateTime::currentMSecsSinceEpoch() - startTime) / 1000.;
        double t0 = t - std::floor(t);
        double b = abs(1. - 2. * t0);
        lighting.setBrightness(b);
        qDebug() << "brightness" << b;
    });

    return app.exec();
}

