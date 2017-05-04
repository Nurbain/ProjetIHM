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
        case kSignalDuration:
            ui->Serveur_MusicTemps->setText(QString::number((int)(params[kParamDuration].toDouble()/60))+":"+QString::number((int)(params[kParamDuration].toDouble())%60));
            break;
        case kSignalTimeChange:
            ui->Serveur_MusicAvancement->setText(QString::number((int)(params[kParamTimeChange].toDouble()/60))+":"+QString::number((int)(params[kParamTimeChange].toDouble())%60));
            break;

    }


}
