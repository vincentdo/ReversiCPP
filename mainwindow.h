#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QObject>
#include "board.h"


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
    void on_pushButton_2_clicked();
    void update_score();
    void on_pushButton_clicked();

    void on_pushButton_Instruction_clicked();

    void on_pushButton_Exit_clicked();

private:
    Ui::MainWindow *ui;
    Board * canvas;
    QTimer * timer;
};

#endif // MAINWINDOW_H
