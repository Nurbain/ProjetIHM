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
};

#endif // MAINWINDOW_H
