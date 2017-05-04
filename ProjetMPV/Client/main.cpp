/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */


#include "mainwindow.h"
#include "sendjsoncommand.h"
#include "client.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Client c;

    //QObject::connect(&c, SIGNAL(signalFromClient(signalType,QVariantMap), &w, SLOT(messageFromClient(signalType,QVariantMap)));
    QObject::connect(&w, SIGNAL(signalFromUI(signalType,QVariantMap)),&c,SLOT(messageFromUI(signalType,QVariantMap)));

    QVariantMap params;

    params[kParamSwitch]=QVariant(true);

    c.messageFromUI(kSignalConnectToServer,params);

    params[kParamNomMusique]="../Musique/vexento-busy-mind.mp3";

    //c.messageFromUI(kSignalChangementMusique,params);


    return a.exec();
}
