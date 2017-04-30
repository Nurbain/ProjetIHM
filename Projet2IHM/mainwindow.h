/* Urbain Nathan & Quentin Wendlig
 * L3S6 CMI Image
 * Projet 2 IHM
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BtnMenu_Music_clicked();

    void on_BtnMenu_Radio_clicked();

    void on_BtnMenu_serveur_clicked();

    void on_BtnMenu_Param_clicked();

    void on_BtnMenu_Attente_clicked();

    void on_BtnMenu_Play_clicked();

    void on_Player_Pause_clicked();

    void on_Player_Play_clicked();

    void on_Player_MinVol_clicked();

    void on_Player_Muet_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
