#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QEvent>
#include <QGuiApplication>
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

class EventFilter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QGuiApplication *application READ application CONSTANT FINAL)
public:
    explicit EventFilter(QObject *parent = nullptr);

    QGuiApplication *application() const;

public slots:
    void installToObject(QObject *object);

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    void makeScreenshot();

signals:
    void buttonClicked();
    void userInactive();

    void applicationChanged();

private:
    QTimer _inactivityTimer;
    QGuiApplication *_application = nullptr;
};

#endif // EVENTFILTER_H
