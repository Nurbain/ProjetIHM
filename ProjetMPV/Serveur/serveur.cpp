/* Urbain Nathan & Quentin Wendling
 * L3S6 CMI Image
 * Projet 2 IHM
 */

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
            //mpv_json->changeMusicOnMPV("test.mp3");
            //qDebug() << "Morceau lancé !";
            QVariantMap params;
            params[kParamSwitch]=QVariant(true);
            params[kParamStatus]=QVariant(kStatusStarted);
            emit signalFromServer(kSignalStatusMessage,params);
        }
        connect(m_server,SIGNAL(newConnection()),this,SLOT(connectionFromClient()));
    }else{
        if(m_client.size() != 0){
            QVariantMap params;
            params[kParamSwitch]=QVariant(false);
            params[kParamStatus]=QVariant(kStatusStarted);
            message(kSignalStatusMessage,params);
            for(int i=0;i<(int)m_client.size();i++){
                m_client.at(i)->abort();
            }
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
    for(int i=0;i<(int)m_client.size();i++){
        sendRequestToSocket(m_client.at(i),sig,params);
    }

}

void Serveur::connectionFromClient(){
    m_client.push_back(m_server->nextPendingConnection());
    QVariantMap params;
    qDebug("Connetion client");
    params[kParamMessage]=QVariant("Client connecté");
    params[kParamStatus]=QVariant(kStatusMessage);
    emit signalFromServer(kSignalStatusMessage,params);
    params[kParamSwitch]=QVariant(true);
    if(!m_lecture)
        emit signalFromServer(kSignalStart,params);
    m_lecture=true;
    connect(m_client.back(), SIGNAL(disconnected()),m_client.back(),SLOT(deleteLater()));
    connect(m_client.back(),SIGNAL(disconnected()),this,SLOT(clientDisconnected()));
}


void Serveur::clientDisconnected(){
    m_client.removeAll((QLocalSocket*)sender());
    QVariantMap params;
    params[kParamMessage]=QVariant("Client déconnecté");
    params[kParamStatus]=QVariant(kStatusMessage);
    params[kParamSwitch]=QVariant(false);
    //emit signalFromServer(kSignalStart,params);
    emit signalFromServer(kSignalStatusMessage,params);
}

void Serveur::clientMessageLoop() {
    while(m_running){
        for(int i=0;i<(int)m_client.size();i++){
            QDataStream in(m_client.at(i));
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
}
