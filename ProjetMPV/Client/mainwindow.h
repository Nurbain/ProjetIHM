#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../Common/common.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void signalFromUI(signalType,QVariantMap);

private:
    Ui::MainWindow *ui;
    void RecuperationMusique ();

public slots:
    void Music_clicked();
private slots:
    void on_Player_Play_clicked();
    void on_Player_Pause_clicked();
    void on_Param_Eng_clicked();
    void on_BtnMenu_Music_clicked();
    void on_BtnMenu_Play_clicked();
    void on_BtnMenu_Attente_clicked();
    void on_BtnMenu_Radio_clicked();
    void on_BtnMenu_serveur_clicked();
    void on_BtnMenu_Param_clicked();
    void on_Param_Fr_clicked();
    void on_Player_Volume_valueChanged(int value);
    void on_Player_Muet_clicked();
    void on_Player_MinVol_clicked();
};

#endif // MAINWINDOW_H
