#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QHistoryState>
#include <QFinalState>
#include <QSignalTransition>
#include <QTimer>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include "sendjsoncommand.h"
#include "../../Common/common.h"

class Automate : public QObject
{
    Q_OBJECT
public:
    explicit Automate(QObject *parent = 0);

private:
    QStateMachine *machine;

    QState *play;
    QState *pause;
    QState *changement;
    QState *changementPlaylist;
    QState *speedUp;

    QState *fonction;
    QHistoryState *fonctionHistory;

    QSignalTransition *dePlay;
    QSignalTransition *dePause;
    QSignalTransition *deChangement;
    QSignalTransition *deChangementPlaylist;
    QSignalTransition *deSpeedUp;


    SendJSONCommand *mpv_json;

    QString currentMusique;
    QString currentPlaylist;


signals:

    void signalMachine(signalType,QVariantMap);
    void signalPause();
    void signalChangement();
    void signalToUI(signalType,QVariantMap);
    void signalChangementPlaylist();
    void signalSpeed();

public slots:

    void messageFromServer(signalType, QVariantMap);
    void start(bool);
    void changementMusique();
    void playMusique();
    void pauseMusique();
    void readSocket();
    void changementPlaylistFct();
    void speedMusique();

};

#endif // AUTOMATE_H
