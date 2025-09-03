#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QEvent>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

class EventFilter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit EventFilter(QObject *parent = nullptr);

public slots:
    void installToObject(QObject *object);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void makeScreenshot();

signals:
    void userInactive();

private:
    QTimer _inactivityTimer;
};

#endif // EVENTFILTER_H
