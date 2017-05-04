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
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QDirIterator dirIterator(Dir, listFilter , QDir::Files , QDirIterator::Subdirectories);
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


void MainWindow::on_Player_Pause_clicked()
{
    ui->Player_Pause->setVisible(false);
    ui->Player_Play->setVisible(true);
    qDebug() << "play";
    QVariantMap params;
    params[kParamSwitch]=true;
    emit signalFromUI(kSignalPause, params);
}

void MainWindow::on_Player_Play_clicked()
{
    ui->Player_Pause->setVisible(true);
    ui->Player_Play->setVisible(false);
    qDebug() << "play";
    QVariantMap params;
    params[kParamSwitch]=false;
    emit signalFromUI(kSignalPause, params);
}
