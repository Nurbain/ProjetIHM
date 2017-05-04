#include "client.h"

Client::Client(QObject *parent) :
  QObject(parent),
  m_socket(new QLocalSocket(this))
{
  qRegisterMetaType<signalType>("signalType");
  m_running=true;
  // Receive loop
  m_clientLoopThread=QtConcurrent::run(this, &Client::serverMessageLoop);
}

Client::~Client() {
  m_socket->disconnectFromServer();
  m_running=false;
  m_clientLoopThread.waitForFinished();
}


void Client::messageFromUI(signalType sig, QVariantMap params) {
  switch(sig){
    case kSignalConnectToServer:
      if (m_socket==NULL) return;
      if (params[kParamSwitch].toBool()) {
          m_socket->abort();
          m_socket->connectToServer(SERVER_NAME);
          if (m_socket->waitForConnected()) {
              QVariantMap params;
              params[kParamSwitch]=QVariant(true);
              params[kParamStatus]=QVariant(kStatusStarted);
              emit signalFromClient(kSignalStatusMessage, params);
            }else { // Non connecté
              qDebug()<<m_socket->errorString();
              QVariantMap params;
              params[kParamStatus]=QVariant(kStatusError);
              params[kParamMessage]=QVariant(m_socket->errorString());
              emit signalFromClient(kSignalStatusMessage, params);
            }
        }else{ // Off
          m_socket->disconnectFromServer();
          QVariantMap params;
          params[kParamSwitch]=QVariant(false);
          params[kParamStatus]=QVariant(kStatusStarted);
          emit signalFromClient(kSignalStatusMessage, params);
        }
      break;
    case kSignalChangementMusique:
      if(m_socket==NULL) return;
      sendRequestToSocket(m_socket,kSignalChangementMusique,params);
      break;
    case kSignalPause:
      sendRequestToSocket(m_socket,kSignalPause,params);
      break;
    case kSignalChangementVolume:
      sendRequestToSocket(m_socket,kSignalChangementVolume,params);
      break;
    default:
      break;
    }
}


void Client::serverMessageLoop() {
  while (m_running){
      QDataStream in(m_socket);
      if (in.atEnd()){ // Rien dans la file d'attente
          QThread::msleep(100); // On attend 1/10s et on continue
          continue;
        }
      QString str=QString(in.device()->readLine());
      if(str=="") continue; // Evitons les lignes vides.
      QByteArray a=str.toUtf8();
      QJsonParseError error;
      QJsonDocument jDoc=QJsonDocument::fromJson(a, &error);
      QJsonObject jsonObject=jDoc.object();
      emit signalFromClient((signalType)jsonObject[kJsonSignal].toInt(), jsonObject[kJsonParams].toObject().toVariantMap());
    }
}
