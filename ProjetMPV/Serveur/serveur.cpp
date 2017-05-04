#include "serveur.h"
#include <QLocalSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDataStream>
#include <QThread>

Serveur::Serveur(QObject *parent) :
    QObject(parent),
    m_server(new QLocalServer(this))
{
    qRegisterMetaType<signalType>("signalType");
    mpv_json = new SendJSONCommand();
    m_running=true;
    m_serverLoopThread=QtConcurrent::run(this, &Serveur::clientMessageLoop);
}

Serveur::~Serveur(){
    m_running=false;
    m_serverLoopThread.waitForFinished();
}

void Serveur::startServer(bool on){
    if(on){
        QString serverName(SERVER_NAME);
        QLocalServer::removeServer(serverName);
        if( !m_server->listen(serverName)) {
            QVariantMap params;
            params[kParamMessage]=QVariant(m_server->errorString());
            params[kParamStatus]=QVariant(kStatusError);
            emit signalFromServer(kSignalStatusMessage,params);
            return;
        }
        else{
            mpv_json->changeMusicOnMPV("test.mp3");
            qDebug() << "Morceau lancé !";
            QVariantMap params;
            params[kParamSwitch]=QVariant(true);
            params[kParamStatus]=QVariant(kStatusStarted);
            emit signalFromServer(kSignalStatusMessage,params);
        }
        connect(m_server,SIGNAL(newConnection()),this,SLOT(connectionFromClient()));
    }else{
        if(m_client){
            QVariantMap params;
            params[kParamSwitch]=QVariant(false);
            params[kParamStatus]=QVariant(kStatusStarted);
            message(kSignalStatusMessage,params);
            m_client->abort();
            emit signalFromServer(kSignalStart,params);
        }
        m_server->close();
        QVariantMap params;
        params[kParamSwitch]=QVariant(false);
        params[kParamStatus]=QVariant(kStatusStarted);
        emit signalFromServer(kSignalStatusMessage,params);
    }
}

void Serveur::message(signalType sig,QVariantMap params){
    sendRequestToSocket(m_client,sig,params);
}

void Serveur::connectionFromClient(){
    if(m_client) return; //Un seul client faudra changer ca
    m_client=m_server->nextPendingConnection();
    QVariantMap params;
    qDebug("Conneton client");
    params[kParamMessage]=QVariant("Client connecté");
    params[kParamStatus]=QVariant(kStatusMessage);
    emit signalFromServer(kSignalStatusMessage,params);
    params[kParamSwitch]=QVariant(true);
    emit signalFromServer(kSignalStart,params);
    connect(m_client, SIGNAL(disconnected()),m_client,SLOT(deleteLater()));
    connect(m_client,SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
}


void Serveur::clientDisconnected(){
    m_client=NULL;
    QVariantMap params;
    params[kParamMessage]=QVariant("Client déconnecté");
    params[kParamStatus]=QVariant(kStatusMessage);
    params[kParamSwitch]=QVariant(false);
    emit signalFromServer(kSignalStart,params);
    emit signalFromServer(kSignalStatusMessage,params);
}

void Serveur::clientMessageLoop() {
    while(m_running){
        QDataStream in(m_client);
        if(in.atEnd()){
            QThread::msleep(100);
            continue;
        }
        QString str=QString(in.device()->readLine());
        if(str=="") continue;
        QByteArray a=str.toUtf8();
        QJsonParseError error;
        QJsonDocument jDoc=QJsonDocument::fromJson(a,&error);
        QJsonObject jsonObject=jDoc.object();
        emit signalFromServer((signalType)jsonObject[kJsonSignal].toInt(), jsonObject[kJsonParams].toObject().toVariantMap());
    }
}
