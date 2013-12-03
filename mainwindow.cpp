#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new Board(this);
    ui->formLayout_2->addWidget(canvas);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    if (canvas->state == PAUSED){
        canvas->state = PLAYING;
        ui->pushButton_2->setText("Pause");
        ui->textBrowser->setText("Playing...");
    }else if (canvas->state == PLAYING){
        canvas->state = PAUSED;
        ui->pushButton_2->setText("Resume");
        ui->textBrowser->setText("Game Paused");
    }else{
        return;
    }
}

void MainWindow::on_pushButton_clicked()
{
    switch (canvas->state){
    case BEFORE_PLAY:
        canvas->state = PLAYING;
        ui->pushButton->setText("Restart");
        ui->textBrowser->setText("Playing...");
        break;
    case AFTER_PLAY:
        return;
    default:
        canvas->state = BEFORE_PLAY;
        ui->pushButton->setText("Start");
        ui->textBrowser->setText("Press \"Start\" to Begin Game");
        canvas->reset();
        return;
    }



}
