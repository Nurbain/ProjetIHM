/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */


#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
