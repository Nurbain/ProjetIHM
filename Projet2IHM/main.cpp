/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */


#include "mainwindow.h"
#include "sendjsoncommand.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SendJSONCommand *test = new SendJSONCommand();

    // Test du changement de morceau
    test->changeMusicOnMPV("test.mp3");
    qDebug() << "Morceau lancé !";

    // Test du changement de volume
    test->changeVolumeOnMPV(50);

    test->quitMPVServer();

    return a.exec();
}
