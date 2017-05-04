#include "bouton.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QMouseEvent>


Bouton::Bouton(QString nomMusique , QWidget *parent) : QWidget(parent)
{
    m_pixmap = new QPixmap(":/icon/Sprite/attente.png");
    nom = nomMusique;
}

Bouton::~Bouton(){
    delete m_pixmap;
}

void Bouton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,20,20 , *m_pixmap);
}

void Bouton::mousePressEvent(QMouseEvent *e)
{
    QVariantMap params;
    params[kParamNomMusique]= nom;
    emit signalFromUI(kSignalChangementMusique,params);
}
