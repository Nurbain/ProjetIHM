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
    changementPlaylist=new QState(fonction);

    deChangementPlaylist=changementPlaylist->addTransition(this,SIGNAL(signalChangementPlaylist()),play);



    dePlay=play->addTransition(this,SIGNAL(signalPause()),pause);
    dePlay=play->addTransition(this,SIGNAL(signalChangement()),changement);
    dePlay=play->addTransition(this,SIGNAL(signalChangementPlaylist()),changementPlaylist);

    dePause=pause->addTransition(this,SIGNAL(signalPause()),play);
    dePause=pause->addTransition(this,SIGNAL(signalChangement()),changement);
    dePause=pause->addTransition(this,SIGNAL(signalChangementPlaylist()),changementPlaylist);

    deChangement=changement->addTransition(this,SIGNAL(signalChangement()),play);

    mpv_json = new SendJSONCommand();

    connect(mpv_json->mpv,SIGNAL(readyRead()),this,SLOT(readSocket()));
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
    QObject::connect(changementPlaylist,SIGNAL(entered()),this,SLOT(changementPlaylistFct()));

    mpv_json->obsProgress();
    mpv_json->obsDuration();
    mpv_json->obsTimeChange();
    mpv_json->obsTitle();
    mpv_json->obsPlayList();

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
            emit signalMachine(kSignalPause,params);
            break;
        case kSignalChangementMusique:
            qDebug("On change");
            currentMusique=params[kParamNomMusique].toString();
            emit signalChangement();
            break;
        case kSignalChangementVolume:
            mpv_json->changeVolumeOnMPV(params[kParamVolume].toInt());
            emit signalMachine(kSignalChangementVolume,params);
            break;
        case kSignalChangementPlaylist:
            currentPlaylist=params[kParamNomPlaylist].toString();
            emit signalChangementPlaylist();
            break;
        case kSignalProgressChange:
            qDebug() << params[kParamProgress].toInt();
            mpv_json->setProgress(params[kParamProgress].toInt());
            break;
        default:
            return;
    }
}

void Automate::changementMusique(){
    mpv_json->changeMusicOnMPV(currentMusique);
    emit signalChangement();
}

void Automate::changementPlaylistFct(){
    mpv_json->changePlaylistOnMPV(currentPlaylist);
    emit signalChangementPlaylist();
}

void Automate::playMusique(){
    mpv_json->setPauseOnMPV(false);
}

void Automate::pauseMusique(){
    //mpv_json->getPosFromMPV();
    mpv_json->setPauseOnMPV(true);
}

void Automate::readSocket(){
    while(mpv_json->mpv->canReadLine()){
        QByteArray line = mpv_json->mpv->readLine().trimmed();
        QVariantMap params;

        QJsonParseError error;
        QJsonDocument jDoc=QJsonDocument::fromJson(line,&error);
        QJsonObject jObj = jDoc.object();
        //qDebug() << QString::fromUtf8(line.constData(),line.length());
        if(jObj["event"] == "property-change"){
            //qDebug("Ah ouais c'est op");
            switch(jObj["id"].toInt()){
                case CHANGE_PROGRESS_PERCENT:
                    //qDebug("Modif progress ");
                    //qDebug() << (int)jObj["data"].toDouble();
                    params[kParamProgress]=QVariant((int)jObj["data"].toDouble());
                    emit signalToUI(kSignalProgress,params);
                    emit signalMachine(kSignalProgress,params);
                    break;
                case CHANGE_DURATION:
                    params[kParamDuration]=QVariant(jObj["data"].toDouble());
                    emit signalToUI(kSignalDuration,params);
                    break;
                case CHANGE_TIMEPOS:
                    params[kParamTimeChange]=QVariant(jObj["data"].toDouble());
                    emit signalToUI(kSignalTimeChange,params);
                    break;
                 case CHANGE_TITLE:
                    params[kParamNomMusique]=QVariant(jObj["data"].toString());
                    emit signalMachine(kSignalChangementMusique,params);
                    emit signalToUI(kSignalChangementMusique,params);
                    break;
                 case CHANGE_PLAYLIST:
                    qDebug() << jObj;
                    int i=0;
                    QJsonArray tmpArray = jObj["data"].toArray();
                    for(;i<(int)tmpArray.size();i++){
                        if(tmpArray.at(i).toObject()["current"].toBool())
                            break;
                    }
                    if(i>0)
                        params[kParamPred]=QVariant(tmpArray[i-1].toObject()["filename"].toString());
                    else
                        params[kParamPred]=QVariant("");
                    if(i<(int)tmpArray.size()-1)
                        params[kParamSuiv]=QVariant(tmpArray[i+1].toObject()["filename"].toString());
                    else
                        params[kParamSuiv]=QVariant("");

                    emit signalToUI(kSignalSetVoisin,params);
                    break;
            }
        }
    }
}

