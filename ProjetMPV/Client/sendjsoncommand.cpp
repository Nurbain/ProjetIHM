#include "sendjsoncommand.h"

SendJSONCommand::SendJSONCommand(QObject *parent) :
    QObject(parent),
    mpv(new QLocalSocket(this))
{
    // Connecting to mpv server
    qDebug() << "Connecting to server....";
    mpv->connectToServer("/tmp/mpv-socket");

    // Verify if we are connected
    if(mpv->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        mpv->error();
    }

}


SendJSONCommand::~SendJSONCommand()
{
    mpv->disconnectFromServer();
}


/**
* Method : sendRequestToMPV
* Param : QJsonObject, msg - message to send to the mpv server
* Desc : Send a JSON request to the mpv server
* Return : void
**/
void SendJSONCommand::sendRequestToMPV(QJsonObject msg)
{
    // Converting the JSON msg into bytes msg
    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";
    if(mpv!=NULL)
    {
        mpv->write(bytes.data(), bytes.length());
        mpv->flush();
    }
}


/**
* Method : changeMusicOnMPV
* Param : QString, musicName - name of the music to play
* Desc : Create the QJsonObject with the music name and send it to the mpv server
* Return : bool, if success return true, else return false
**/
void SendJSONCommand::changeMusicOnMPV(QString musicName)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("loadfile");
    jsonArr.append(musicName);

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);

}


/**
* Method : changeVolumeOnMPV
* Param : int, newVolume - new volume to send to MPV server
* Desc : Create the QJsonObject with the volume value and send it to the mpv server
* Return : void
**/
void SendJSONCommand::changeVolumeOnMPV(int newVolume)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("set_property");
    jsonArr.append("volume");
    jsonArr.append(newVolume);

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);

}


/**
* Method : setPauseOnMPV
* Param : bool, newValue - the new boolean value to set
* Desc : Create the QJsonObject with the command to set pause on the mpv server
* Return : void
**/
void SendJSONCommand::setPauseOnMPV(bool newValue)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("set_property");
    jsonArr.append("pause");
    jsonArr.append(newValue);

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);

}


/**
* Method : setStopOnMPV
* Param : void
* Desc : Stop the current played music
* Return : void
**/
void SendJSONCommand::setStopOnMPV()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("stop");

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);
}


/**
* Method : quitMPVServer
* Param : void
* Desc : shut down and quit the MPV server
* Return : void
**/
void SendJSONCommand::quitMPVServer()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("quit");

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);
}


/**
* Method : getVolumeFromMPV
* Param : void
* Desc : ask for the volume value to MPV
* Return : int, volume value
**/
void SendJSONCommand::getVolumeFromMPV()
{
    // Send the request to mpv
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("get_property");
    jsonArr.append("volume");

    jsonObject["command"]=jsonArr;

    SendJSONCommand::sendRequestToMPV(jsonObject);


    // Get the result of the call on MPV server
    QByteArray line = mpv->readLine().trimmed();

    // If we got something
    QJsonParseError error;
    QJsonDocument jDoc = QJsonDocument::fromJson(line, &error);
    QJsonObject jObj = jDoc.object();
    qDebug() << jObj["data"].toString();
    qDebug() << jObj["error"].toString();
}










