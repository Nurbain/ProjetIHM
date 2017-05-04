#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QDir>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIcon>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    RecuperationMusique ();
    RecuperationRadio();

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
            qDebug() << "PauseEasy";
            setPause();
            break;
        case kSignalChangementMusique:
            setInfos(params[kParamNomMusique].toString());
            setPlayChange();
            break;
        case kSignalChangementVolume:
            setVolume(params[kParamVolume].toInt());
            break;
        case kSignalProgress:
            setProgress(params[kParamProgress].toInt());
            break;
        case kSignalDuration:
            qDebug() << "ici";
            setInfoDure(QString::number((int)(params[kParamDuration].toDouble()/60))+":"+QString::number((int)(params[kParamDuration].toDouble())%60));
            break;
        case kSignalTimeChange:
            qDebug() << "ici2";
            setInfoAdvance(QString::number((int)(params[kParamTimeChange].toDouble()/60))+":"+QString::number((int)(params[kParamTimeChange].toDouble())%60));
            break;
        default:
            return;
    }
}


// On sélectionne le répertoire à partir duquel on va rechercher les fichiers MP3
void MainWindow::RecuperationMusique ()
{

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


    QStringList nomFichier;

    // Tant qu'on n'est pas arrivé à la fin de l'arborescence...

    QVBoxLayout *lay = new QVBoxLayout(this);
    while(dirIterator.hasNext())
    {

        // ...on va au prochain fichier correspondant à notre filtre
        QFileInfo file(dirIterator.next());
        //fileList << dirIterator.fileInfo();
        nomFichier << file.fileName();

        QString nom = file.fileName();
        nom.resize(nom.size()-4);
        //QIcon icon = new QIcon);
        QPushButton *newbtn = new QPushButton (nom);
        lay->addWidget(newbtn);
        newbtn->text() = nom;
        QObject::connect(newbtn, SIGNAL (clicked(bool)) ,this , SLOT(Music_clicked()));

    }
    ui->ContentMusique->setLayout(lay);

    QDir dir_path(Dir);
    dir_path.setFilter((QDir::AllDirs));
    QStringList Alldir = dir_path.entryList();
    QVBoxLayout *lay2 = new QVBoxLayout(this);
    for(int i = 0 ; i<Alldir.size() ; i++)
    {

        if(Alldir.at(i) != "." && Alldir.at(i) != "..")
        {
            QString newpath = Dir+"/"+Alldir.at(i);
            QDir::setCurrent(newpath);
            QDirIterator dirIterator2(newpath, listFilter , QDir::Files | QDir::NoSymLinks );

            QPixmap image(":/icon/Sprite/dropdown.png");
            QIcon icon(image);
            QPushButton *playList = new QPushButton (Alldir.at(i));
            playList->setIcon(icon);
            lay2->addWidget(playList);
            QObject::connect(playList, SIGNAL (clicked(bool)) ,this , SLOT(PlayList_clicked()));

            while(dirIterator2.hasNext())
            {
                QFileInfo file(dirIterator2.next());
                QLabel *titre  =new QLabel(file.fileName());
                lay2->addWidget(titre);
                QString nom = file.fileName();
                nom.resize(nom.size()-4);
                titre->setText("- "+nom);

            }

        }


    }
    ui->ContentPlayList->setLayout(lay2);

}

void MainWindow::RecuperationRadio ()
{
     QString selectDir = QDir::currentPath() ;
     QString path = "/../../Radio";
     QString Dir= selectDir + path;

     QStringList listFilter;
     listFilter << "*.m3u";

     QDirIterator dirIterator(Dir, listFilter , QDir::Files);
     QStringList nomFichier;


     QVBoxLayout *lay = new QVBoxLayout(this);
     while(dirIterator.hasNext())
     {

         // ...on va au prochain fichier correspondant à notre filtre
         QFileInfo file(dirIterator.next());
         //fileList << dirIterator.fileInfo();
         nomFichier << file.fileName();

         QString nom = file.fileName();
         nom.resize(nom.size()-4);
         QPushButton *newbtn = new QPushButton (nom);
         lay->addWidget(newbtn);
         newbtn->text() = nom;
         QObject::connect(newbtn, SIGNAL (clicked(bool)) ,this , SLOT(Radio_clicked()));
     }
     ui->ContentRadio->setLayout(lay);
}

void MainWindow::Music_clicked()
{
    QObject * emetteur = sender();
    QPushButton * cast = qobject_cast<QPushButton*>(emetteur);
    QString nom = cast->text() + ".mp3";
    QVariantMap params;
    params[kParamNomMusique]=nom;
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

void MainWindow::Radio_clicked()
{
    QObject * emetteur = sender();
    QPushButton * cast = qobject_cast<QPushButton*>(emetteur);
    QString nom = "../Radio/"+ cast->text() + ".m3u";
    qDebug() << nom;
    QVariantMap params;
    params[kParamNomPlaylist]=nom;
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

void MainWindow::setPlayChange()
{
    ui->Player_Play->setVisible(false);
    ui->Player_Pause->setVisible(true);
}

void MainWindow::setInfos(QString nom)
{
    ui->Player_Chanson->setText(nom);
}

void MainWindow::setInfoDure(QString dure)
{
    ui->player_Duration->setText(dure);
}

void MainWindow::setInfoAdvance(QString temps)
{
    ui->Player_Temps->setText(temps);
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
    ui->BtnMenu_Param->setText("Settings");
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
    ui->BtnMenu_Param->setText("Parametre");
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
    ui->BtnMenu_Param->setText("Paràmetro");
    ui->Param_Serveur->setText("Connexion");
    ui->Param_Langue->setText("lenga");
}


void MainWindow::on_BtnMenu_Music_clicked()
{
    ui->TabInteraction->setCurrentIndex(0);
}

void MainWindow::on_BtnMenu_Play_clicked()
{
    ui->TabInteraction->setCurrentIndex(3);
}

void MainWindow::on_BtnMenu_Attente_clicked()
{
    ui->TabInteraction->setCurrentIndex(4);
}


void MainWindow::on_BtnMenu_Radio_clicked()
{
     ui->TabInteraction->setCurrentIndex(1);
}


void MainWindow::on_BtnMenu_Param_clicked()
{
     ui->TabInteraction->setCurrentIndex(2);
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
