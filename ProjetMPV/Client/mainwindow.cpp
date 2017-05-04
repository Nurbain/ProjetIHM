#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDir>
#include <QPushButton>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RecuperationMusique ();

    ui->Player_Pause->setVisible(false);
    ui->Player_Muet->setVisible(false);
    ui->Panel->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::messageFromClient(signalType sig, QVariantMap params)
{
    switch(sig){
        case kSignalStart:
            setConnection();
            break;
        case kSignalPause:
            setPause();
            break;
        case kSignalChangementMusique:

            break;
        case kSignalChangementVolume:
            setVolume(params[kParamVolume].toInt());
            break;
        case kSignalProgress:
            setProgress(params[kParamProgress].toInt());
            break;
        default:
            return;
    }
}


// On sélectionne le répertoire à partir duquel on va rechercher les fichiers MP3
void MainWindow::RecuperationMusique ()
{
    qDebug() << "la";

    // On sélectionne le répertoire à partir duquel on va rechercher les fichiers MP3
    QString selectDir = QDir::currentPath() ;
    QString path = "/../../Musique";
    QString Dir= selectDir + path;
    // On remplit une QStringList avec chacun des filtres désirés ici "*.mp3"
    QStringList listFilter;
    listFilter << "*.mp3";


    // On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un répertoire et ses sous-répertoires.
    // De plus, on spécifie le filtre qui nous permettra de récupérer uniquement les fichiers du type souhaité.
    QDirIterator dirIterator(Dir, listFilter , QDir::Files);
    // Variable qui contiendra tous les fichiers correspondant à notre recherche
    QFileInfoList fileList;

    QStringList nomFichier;
    int x = 90;
    int y = 20;
    // Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(dirIterator.hasNext())
    {

        // ...on va au prochain fichier correspondant à notre filtre
        QFileInfo file(dirIterator.next());
        //fileList << dirIterator.fileInfo();
        nomFichier << file.fileName();
        qDebug() << file.fileName();

        QPushButton *newbtn = new QPushButton (file.fileName(), ui->Tab_Musique);
        QString nom = newbtn->text();
        QObject::connect(newbtn, SIGNAL (clicked(bool)) ,this , SLOT(Music_clicked()));
        newbtn->setGeometry(x,y,461,25);

        y = y+30;
    }

    qDebug() << "la2";
    QDir dir_path(Dir);
    dir_path.setFilter((QDir::AllDirs));
    QStringList Alldir = dir_path.entryList();
    int xDir = 110 , yDir = 20 , largeDir = 200, hauteurDir = 20;
    for(int i = 0 ; i<Alldir.size() ; i++)
    {

        if(Alldir.at(i) != "." && Alldir.at(i) != "..")
        {
            QString newpath = Dir+"/"+Alldir.at(i);
            QDir::setCurrent(newpath);
            QDirIterator dirIterator2(newpath, listFilter , QDir::Files | QDir::NoSymLinks );

            QPushButton *playList = new QPushButton (Alldir.at(i), ui->Tab_PlayList);
            playList->setGeometry(xDir,yDir , largeDir , hauteurDir);
            QObject::connect(playList, SIGNAL (clicked(bool)) ,this , SLOT(PlayList_clicked()));

            while(dirIterator2.hasNext())
            {
                yDir = yDir+20;
                QFileInfo file(dirIterator2.next());
                QLabel *titre  =new QLabel(file.fileName() , ui->Tab_PlayList);
                titre->setText("- "+file.fileName());
                qDebug() << titre->text();
                titre->setGeometry(xDir+30,yDir , largeDir+200 , hauteurDir);


            }
            yDir = yDir+50;
        }


    }

}


void MainWindow::Music_clicked()
{
    QObject * emetteur = sender();
    QPushButton * cast = qobject_cast<QPushButton*>(emetteur);
    qDebug() << cast->text();;
    QVariantMap params;
    params[kParamNomMusique]=cast->text();
    emit signalFromUI(kSignalChangementMusique, params);
}

void MainWindow::PlayList_clicked()
{
    QObject * emetteur = sender();
    QPushButton * cast = qobject_cast<QPushButton*>(emetteur);
    qDebug() << cast->text();;
    QVariantMap params;
    params[kParamNomPlaylist]=cast->text();
    emit signalFromUI(kSignalChangementPlaylist, params);
}


//MessageFromClient -> SetUI

void MainWindow::setPause()
{
    const bool visible = ui->Player_Pause->isVisible();
    if(visible)
    {
        ui->Player_Pause->setVisible(false);
        ui->Player_Play->setVisible(true);
    }
    else
    {
        ui->Player_Pause->setVisible(true);
        ui->Player_Play->setVisible(false);
    }
}

void MainWindow::setVolume(int value)
{
    ui->Player_Volume->disconnect(ui->Player_Volume , SIGNAL(valueChanged(int)) , this , SLOT(on_Player_Volume_valueChanged(int)));
    if(value != 0)
    {
        ui->Player_Volume->setValue(value);
        ui->Player_Muet->setVisible(false);
        ui->Player_MinVol->setVisible(true);
    }
    else
    {
        ui->Player_Muet->setVisible(true);
        ui->Player_MinVol->setVisible(false);
    }
    ui->Player_Volume->connect(ui->Player_Volume , SIGNAL(valueChanged(int)) , this , SLOT(on_Player_Volume_valueChanged(int)));
}

void MainWindow::setConnection()
{
    ui->Param_On->setChecked(true);
    ui->Param_Off->setChecked(false);
    ui->Panel->setCurrentIndex(0);
}

void MainWindow::setProgress(int value)
{
    if(canModif)
        ui->Player_ProgressBar->setValue(value);
}

void MainWindow::on_Player_Pause_clicked()
{
    /*ui->Player_Pause->setVisible(false);
    ui->Player_Play->setVisible(true);*/
    qDebug() << "pause";
    QVariantMap params;
    params[kParamSwitch]=false;
    emit signalFromUI(kSignalPause, params);
}

void MainWindow::on_Player_Play_clicked()
{
    /*ui->Player_Pause->setVisible(true);
    ui->Player_Play->setVisible(false);*/
    qDebug() << "play";
    QVariantMap params;
    params[kParamSwitch]=true;
    emit signalFromUI(kSignalPause, params);
}

void MainWindow::on_Param_Eng_clicked()
{
    //Menu
    ui->BtnMenu_Music->setText("Music");
    ui->BtnMenu_Play->setText("PlayList");
    ui->BtnMenu_Radio->setText("Radio");
    ui->BtnMenu_Attente->setText("Waiting");
    ui->BtnMenu_serveur->setText("Server");
    ui->BtnMenu_Param->setText("Settings");
    ui->Param_other->setText("high quality");
    ui->Param_Serveur->setText("Connexion");
    ui->Param_Langue->setText("Language");
}

void MainWindow::on_Param_Fr_clicked()
{
    //Menu
    ui->BtnMenu_Music->setText("Musique");
    ui->BtnMenu_Play->setText("PlayList");
    ui->BtnMenu_Radio->setText("Radio");
    ui->BtnMenu_Attente->setText("Attente");
    ui->BtnMenu_serveur->setText("Serveur");
    ui->BtnMenu_Param->setText("Parametre");
    ui->Param_other->setText("Haute qualité");
    ui->Param_Serveur->setText("Serveur");
    ui->Param_Langue->setText("Langue");
}

void MainWindow::on_Param_Esp_clicked()
{
    //Menu
    ui->BtnMenu_Music->setText("Mùsica");
    ui->BtnMenu_Play->setText("PlayList");
    ui->BtnMenu_Radio->setText("Radio");
    ui->BtnMenu_Attente->setText("Espera");
    ui->BtnMenu_serveur->setText("Servidor");
    ui->BtnMenu_Param->setText("Paràmetro");
    ui->Param_other->setText("Alta calidad");
    ui->Param_Serveur->setText("Connexion");
    ui->Param_Langue->setText("lenga");
}


void MainWindow::on_BtnMenu_Music_clicked()
{
    ui->TabInteraction->setCurrentIndex(0);
}

void MainWindow::on_BtnMenu_Play_clicked()
{
    ui->TabInteraction->setCurrentIndex(4);
}

void MainWindow::on_BtnMenu_Attente_clicked()
{
    ui->TabInteraction->setCurrentIndex(5);
}


void MainWindow::on_BtnMenu_Radio_clicked()
{
     ui->TabInteraction->setCurrentIndex(1);
}


void MainWindow::on_BtnMenu_serveur_clicked()
{
     ui->TabInteraction->setCurrentIndex(2);
}

void MainWindow::on_BtnMenu_Param_clicked()
{
     ui->TabInteraction->setCurrentIndex(3);
}

void MainWindow::on_Player_Volume_valueChanged(int value)
{
    QVariantMap params;
    params[kParamVolume]=value;
    emit signalFromUI(kSignalChangementVolume, params);
}

void MainWindow::on_Player_Muet_clicked()
{
    ui->Player_Muet->setVisible(false);
    ui->Player_MinVol->setVisible(true);
    int value = ui->Player_Volume->value();
    QVariantMap params;
    params[kParamVolume]=value;
    emit signalFromUI(kSignalChangementVolume, params);
}

void MainWindow::on_Player_MinVol_clicked()
{
    ui->Player_Muet->setVisible(true);
    ui->Player_MinVol->setVisible(false);
    QVariantMap params;
    params[kParamVolume]=0;
    emit signalFromUI(kSignalChangementVolume, params);
}


void MainWindow::on_Param_On_clicked()
{
    QVariantMap params;
    params[kParamSwitch]=QVariant(true);
    emit signalFromUI(kSignalConnectToServer,params);
}

void MainWindow::on_Param_Off_clicked()
{
    QVariantMap params;
    params[kParamSwitch]=QVariant(false);
    emit signalFromUI(kSignalConnectToServer,params);
     ui->Panel->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    QVariantMap params;
    params[kParamSwitch]=QVariant(true);
    emit signalFromUI(kSignalConnectToServer,params);

    ui->Param_On->setChecked(true);
    ui->Param_Off->setChecked(false);
    ui->Panel->setCurrentIndex(0);
}

void MainWindow::onReleaseProgress(){
    QVariantMap params;
    params[kParamProgress]=QVariant(ui->Player_ProgressBar->value());
    emit signalFromUI(kSignalProgressChange,params);
    canModif=true;
}

void MainWindow::on_Player_ProgressBar_sliderPressed()
{
    canModif=false;
}
