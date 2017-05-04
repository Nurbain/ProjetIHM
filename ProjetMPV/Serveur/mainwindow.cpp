/* Urbain Nathan & Quentin Wendling
 * L3S6 CMI Image
 * Projet 2 IHM
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Serveur_MusicAfter->setVisible(false);
    ui->Serveur_MusicBefore->setVisible(false);
    ui->label->setVisible(false);
    ui->label1->setVisible(false);
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
         case kSignalSetVoisin:
            if(params[kParamPred].toString() != ""){
                ui->label->setVisible(true);
                ui->Serveur_MusicBefore->setText(params[kParamPred].toString());
                ui->Serveur_MusicBefore->setVisible(true);
            }else{
                ui->label->setVisible(false);
                ui->Serveur_MusicBefore->setVisible(false);
            }

            if(params[kParamSuiv].toString() != ""){
                ui->label1->setVisible(true);
                ui->Serveur_MusicAfter->setText(params[kParamSuiv].toString());
                ui->Serveur_MusicAfter->setVisible(true);
            }else{
                ui->label1->setVisible(false);
                ui->Serveur_MusicAfter->setVisible(false);
            }

            break;

    }


}
