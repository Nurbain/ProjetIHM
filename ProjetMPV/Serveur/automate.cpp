#include "automate.h"
#include <QThread>

Automate::Automate(QObject *parent) : QObject(parent)
{
    machine =new QStateMachine(this);
    fonction=new QState(machine);
    fonctionHistory=new QHistoryState(fonction);

    play=new QState(fonction);
    pause=new QState(fonction);
    changement=new QState(fonction);

    dePlay=play->addTransition(this,SIGNAL(signalPause()),pause);
    dePlay=play->addTransition(this,SIGNAL(signalChangement()),changement);

    dePause=pause->addTransition(this,SIGNAL(signalPause()),play);
    dePause=pause->addTransition(this,SIGNAL(signalChangement()),changement);

    deChangement=changement->addTransition(this,SIGNAL(signalChangement()),play);

    mpv_json = new SendJSONCommand();
}


void Automate::start(bool on){
    if(!on){
        mpv_json->quitMPVServer();
        return;
    }

    qDebug("start automate");
    QObject::connect(changement,SIGNAL(entered()),this,SLOT(changementMusique()));
    QObject::connect(play,SIGNAL(entered()),this,SLOT(playMusique()));
    QObject::connect(pause,SIGNAL(entered()),this,SLOT(pauseMusique()));

    fonction->setInitialState(pause);
    machine->setInitialState(fonction);
    machine->start();
}

void Automate::messageFromServer(signalType sig, QVariantMap params){
    switch(sig){
        case kSignalStart:
            start(params[kParamSwitch].toBool());
            break;
        case kSignalPause:
            emit signalPause();
            break;
        case kSignalChangementMusique:
            currentMusique=params[kParamNomMusique].toString();
            emit signalChangement();
            break;
        case kSignalChangementVolume:
            mpv_json->changeVolumeOnMPV(params[kParamVolume].toInt());
            break;
        default:
            return;
    }
}

void Automate::changementMusique(){
    mpv_json->changeMusicOnMPV(currentMusique);
    emit signalChangement();
}

void Automate::playMusique(){

    mpv_json->setPauseOnMPV(false);
}

void Automate::pauseMusique(){
    mpv_json->getPosFromMPV();
    mpv_json->setPauseOnMPV(true);
}

