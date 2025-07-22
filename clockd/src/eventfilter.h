#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QEvent>
#include <QObject>
#include <QTimer>

class EventFilter : public QObject
{
    Q_OBJECT
public:
    explicit EventFilter(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

    void makeScreenshot();

signals:
    void userInactive();

private:
    QTimer _inactivityTimer;
};

#endif // EVENTFILTER_H
