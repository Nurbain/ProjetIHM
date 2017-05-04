#include <QJsonValue>
#include <QVariant>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "common.h"

const char kParams[]="params";
const char kParamSwitch[]="switch";
const char kParamStatus[]="status";
const char kParamMessage[]="message";
const char kParamNomMusique[]="NomMusique";
const char kParamVolume[]="volume";

const char kJsonSignal[]="signal";
const char kJsonParams[]="params";


QJsonValue toJsonValue(QVariant data){
    switch (data.userType()) {
    case QVariant::Int:
        return QJsonValue(data.toInt());
        break;
    case QVariant::String:
        return QJsonValue(data.toString());
        break;
    case QVariant::Bool:
        return QJsonValue(data.toBool());
        break;
    case QVariant::Double:
        return QJsonValue(data.toDouble());
        break;
    default:
        return QJsonValue(0);
        break;
    }
}

void sendRequestToSocket(QLocalSocket *socket,signalType sig,QVariantMap params){
    QJsonObject jsonObject;
    jsonObject["signal"]=sig;
    jsonObject[kParams]=QJsonObject::fromVariantMap(params);
    QByteArray bytes = QJsonDocument(jsonObject).toJson(QJsonDocument::Compact)+"\n";
    if (socket != NULL){
        socket->write(bytes.data(),bytes.length());
        socket->flush();
    }
}
