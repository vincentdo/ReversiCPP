#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QMessageBox>
#include <QApplication>
#include "instructions.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Reversi");
    canvas = new Board(this);
    ui->formLayout_2->addWidget(canvas);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_score()));
    timer->start();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_score()
{
    QString black = "Black Score: ";
    black.append(QString::number(canvas->count_black));
    QString white = "White Score: ";
    white.append(QString::number(canvas->count_white));
    switch (canvas->state){
    case BEFORE_PLAY:
        ui->pushButton->setText("Start");
        ui->pushButton_2->setText("Pause");
        ui->textBrowser->setText("Press \"Start\" to Begin Game");
        ui->textBrowserMovelist->clear();
        canvas->reset();
        break;

    case PLAYING:
        ui->pushButton->setText("ReStart");
        ui->pushButton_2->setText("Pause");
        ui->textBrowser->setText("Game Paused");
        if(canvas->player == Qt::black)
            ui->textBrowser->setText("Black's Turn");
        else
            ui->textBrowser->setText("White's Turn");
        if (canvas->NewMoveAdded){
            ui->textBrowserMovelist->append(canvas->newmove);
            canvas->NewMoveAdded = false;
        }
        break;



    case PAUSED:
        ui->pushButton_2->setText("Resume");
        ui->textBrowser->setText("Game Paused");
        break;

    case AFTER_PLAY:
        ui->pushButton->setText("ReStart");
        if(canvas->count_black > canvas->count_white)
            ui->textBrowser->setText("Black Wins!");
        else if(canvas->count_black < canvas->count_white)
            ui->textBrowser->setText("White Wins!");
        else
            ui->textBrowser->setText("Draw!");
        break;
    default:
        return;

    }
    ui->textBrowserBlack->setText(black);
    ui->textBrowserWhite->setText(white);
    ui->textBrowser->setAlignment(Qt::AlignCenter);
    ui->textBrowserBlack->setAlignment(Qt::AlignCenter);
    ui->textBrowserWhite->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_pushButton_2_clicked()
{
    if (canvas->state == PAUSED){
        canvas->state = PLAYING;       
    }else if (canvas->state == PLAYING){
        canvas->state = PAUSED;
    }else
        return;
}

void MainWindow::on_pushButton_clicked()
{
    switch (canvas->state){
    case BEFORE_PLAY:
        canvas->state = PLAYING;
        break;
    case AFTER_PLAY:
        canvas->state = BEFORE_PLAY;
        break;
    default:
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Restart?", "Are you sure to restart the game?",
                                        QMessageBox::Yes|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
            canvas->state = BEFORE_PLAY;
        else
            return;

    }
}

void MainWindow::on_pushButton_Instruction_clicked()
{
    Instructions instructions;
    instructions.setModal(true);
    instructions.exec();
}
void MainWindow::on_pushButton_Exit_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Quit?", "Are you sure to quit the game?",
                                    QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
    else
        return;
}
