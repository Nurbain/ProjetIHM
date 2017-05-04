#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::message(signalType sig, QVariantMap params){

    switch(sig){
        case kSignalChangementMusique:
            qDebug("Modif IHM");
            ui->Serveur_MusicNow->setText(params[kParamNomMusique].toString());
            break;
        case kSignalProgress:
            ui->Serveur_Slider->setSliderPosition(params[kParamProgress].toInt());
            break;

    }


}
