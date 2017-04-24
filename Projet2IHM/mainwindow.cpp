#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //InitBtn
    ui->TabInteraction->setCurrentIndex(0);
    ui->Player_Pause->setVisible(false);
    ui->Player_Muet->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_BtnMenu_Music_clicked()
{
    ui->TabInteraction->setCurrentIndex(0);
}

void MainWindow::on_BtnMenu_Radio_clicked()
{
    ui->TabInteraction->setCurrentIndex(1);}

void MainWindow::on_BtnMenu_serveur_clicked()
{
    ui->TabInteraction->setCurrentIndex(2);
}

void MainWindow::on_BtnMenu_Param_clicked()
{
    ui->TabInteraction->setCurrentIndex(3);
}

void MainWindow::on_BtnMenu_Play_clicked()
{
    ui->TabInteraction->setCurrentIndex(4);
}

void MainWindow::on_BtnMenu_Attente_clicked()
{
     ui->TabInteraction->setCurrentIndex(5);
}

void MainWindow::on_Player_Pause_clicked()
{
    ui->Player_Pause->setVisible(false);
    ui->Player_Play->setVisible(true);
}

void MainWindow::on_Player_Play_clicked()
{
    ui->Player_Pause->setVisible(true);
    ui->Player_Play->setVisible(false);
}

void MainWindow::on_Player_Muet_clicked()
{
    ui->Player_MinVol->setVisible(true);
    ui->Player_Muet->setVisible(false);
}

void MainWindow::on_Player_MinVol_clicked()
{
    ui->Player_MinVol->setVisible(false);
    ui->Player_Muet->setVisible(true);
}
