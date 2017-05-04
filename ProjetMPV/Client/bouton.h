#ifndef BOUTON_H
#define BOUTON_H

#include <QWidget>
#include <QPainter>
#include "QPixmap"
#include <QMainWindow>
#include "../../Common/common.h"
#include "mainwindow.h"
#include "client.h"

class Bouton : public QWidget
{
    Q_OBJECT

public:
    explicit Bouton(QString nom , QWidget *parent = 0);
    ~Bouton();

private:
    QPixmap *m_pixmap;
    QString nom;

signals:
    void signalFromUI(signalType,QVariantMap);

public slots:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
};

#endif // BOUTON_H
