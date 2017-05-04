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
}

MainWindow::~MainWindow()
{
    delete ui;
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
