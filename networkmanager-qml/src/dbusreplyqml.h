#ifndef DBUSREPLYQML_H
#define DBUSREPLYQML_H

#include <QDBusPendingReply>
#include <QQmlEngine>

class DBusReply : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged FINAL)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)

public:
    enum Status { Init, Pending, Successful, Failed };
    Q_ENUM(Status);

    explicit DBusReply(const QDBusPendingCall &reply, QObject *parent = nullptr);

    QVariant value() const;
    Status status() const;

signals:
    void statusChanged();
    void valueChanged();
    void finished();

protected:
    void refreshState();
    void setStatus(const Status &newStatus);
    void setValue(const QVariant &newValue);

private:
    QVariant _value;
    Status _status;
    QDBusPendingCall _reply;
};

#endif // DBUSREPLYQML_H
