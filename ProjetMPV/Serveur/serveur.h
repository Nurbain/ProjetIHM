#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QtConcurrent/QtConcurrent>
#include "../../Common/common.h"


class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(QObject *parent = 0);
    ~Serveur();

private:
    QLocalServer *m_server;
    QLocalSocket *m_client=NULL;
    void clientMessageLoop();
    bool m_running;
    QFuture<void> m_serverLoopThread;

signals:
    void signalFromServer(signalType, QVariantMap);

public slots:
    void message(signalType, QVariantMap);
    void startServer(bool);
    void connectionFromClient();
    void clientDisconnected();
};

#endif // SERVEUR_H
