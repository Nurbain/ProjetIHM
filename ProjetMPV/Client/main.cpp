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

    QVariantMap params;

    params[kParamSwitch]=QVariant(true);

    c.messageFromUI(kSignalConnectToServer,params);


    return a.exec();
}
