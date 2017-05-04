#ifndef SENDJSONCOMMAND_H
#define SENDJSONCOMMAND_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


class SendJSONCommand : public QObject
{
    Q_OBJECT
public:
    explicit SendJSONCommand(QObject *parent = 0);
    ~SendJSONCommand();
    void changeMusicOnMPV(QString musicName);
    void changeVolumeOnMPV(int newVolume);
    void setPauseOnMPV(bool newValue);
    void setStopOnMPV();
    void quitMPVServer();
    void getVolumeFromMPV();
    void obsProgress();
    void getPosFromMPV();
    void obsDuration();
    void obsTimeChange();
    void setProgress(int newProgress);
    void changePlaylistOnMPV(QString playlistName);
    QLocalSocket *mpv=NULL;

private:
    void sendRequestToMPV(QJsonObject msg);
};

#endif // SENDJSONCOMMAND_H
