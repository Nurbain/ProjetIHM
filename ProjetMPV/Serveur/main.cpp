#include "mainwindow.h"
#include "serveur.h"
#include "sendjsoncommand.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //SendJSONCommand *test = new SendJSONCommand();

    // Test du changement de morceau
    //test->changeMusicOnMPV("test.mp3");
    //qDebug() << "Morceau lancé !";

    // Test du changement de volume
    //test->changeVolumeOnMPV(50);

    //test->quitMPVServer();

    Serveur server;

    server.startServer(true);

    return a.exec();
}
