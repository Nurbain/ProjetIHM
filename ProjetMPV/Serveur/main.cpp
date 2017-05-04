/* Urbain Nathan & Quentin Wendling
 * L3S6 CMI Image
 * Projet 2 IHM
 */

#include "mainwindow.h"
#include "serveur.h"
#include "automate.h"
#include "sendjsoncommand.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    /*Il faut lancer mpv dans le repertoire Musique avec
     * mpv --idle --quiet --input-ipc-server=/tmp/mpv-socket &*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Automate automate;

    Serveur server;

    QObject::connect(&automate,SIGNAL(signalMachine(signalType,QVariantMap)),&server,SLOT(message(signalType,QVariantMap)));
    QObject::connect(&server,SIGNAL(signalFromServer(signalType,QVariantMap)),&automate,SLOT(messageFromServer(signalType,QVariantMap)));
    QObject::connect(&server, SIGNAL(signalFromServer(signalType,QVariantMap)), &w, SLOT(message(signalType,QVariantMap)));
    QObject::connect(&automate,SIGNAL(signalToUI(signalType,QVariantMap)),&w,SLOT(message(signalType,QVariantMap)));


    server.startServer(true);



    return a.exec();
}
