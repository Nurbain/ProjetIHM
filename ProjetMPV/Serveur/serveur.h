#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QList>
#include <vector>
#include <QtConcurrent/QtConcurrent>
#include "sendjsoncommand.h"
#include "../../Common/common.h"

using namespace std;


class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(QObject *parent = 0);
    ~Serveur();

private:
    QLocalServer *m_server;
    QList<QLocalSocket*> m_client;
    //QLocalSocket *m_client=NULL;
    void clientMessageLoop();
    bool m_running;
    QFuture<void> m_serverLoopThread;
    SendJSONCommand *mpv_json;
    bool m_lecture=false;

signals:
    void signalFromServer(signalType, QVariantMap);

public slots:
    void message(signalType, QVariantMap);
    void startServer(bool);
    void connectionFromClient();
    void clientDisconnected();
};

#endif // SERVEUR_H
