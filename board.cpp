#include "board.h"
#include <stdio.h>
#include <iostream>

using namespace std;

bool lastPass = false;
bool bothPass = false;

Qt::GlobalColor black = Qt::black;
Qt::GlobalColor white = Qt::white;

int movecount = 0;

Board::Board(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(SIDE_LENGTH+1, SIDE_LENGTH +1);
    state = BEFORE_PLAY;
    reset();
}


void Board::reset(){
    for (int i = 0; i < SQUARE_PER_SIDE; i++){
        for (int j = 0; j < SQUARE_PER_SIDE; j++){
            pieces[i][j] = EMPTY;
        }
    }
    pieces[SQUARE_PER_SIDE/2][SQUARE_PER_SIDE/2] = WHITE;
    pieces[SQUARE_PER_SIDE/2 - 1][SQUARE_PER_SIDE/2] = BLACK;
    pieces[SQUARE_PER_SIDE/2][SQUARE_PER_SIDE/2 - 1] = BLACK;
    pieces[SQUARE_PER_SIDE/2 - 1][SQUARE_PER_SIDE/2 - 1] = WHITE;
    player = black;
    checkForPossibleMove();
    lastPass = false;
    bothPass = false;
    NewMoveAdded = false;
    count_black = 0;
    count_white = 0;
    update();
}

void Board::paintEvent(QPaintEvent *){
    QPainter p(this);
    //Draw the board
    p.drawRect(0, 0, SIDE_LENGTH, SIDE_LENGTH);
    //Repaint the pieces each time
    for (int i = 0; i < SQUARE_PER_SIDE; i++){
        for (int j = 0; j < SQUARE_PER_SIDE; j++){
            p.drawRect(i * SIZE_OF_SQUARE,j * SIZE_OF_SQUARE, SIZE_OF_SQUARE, SIZE_OF_SQUARE);
            p.fillRect(i * SIZE_OF_SQUARE + 1, j * SIZE_OF_SQUARE + 1, SIZE_OF_SQUARE - 1, SIZE_OF_SQUARE - 1, Qt::green);
            if (legalMoves[i][j] == 1){
                p.fillRect(i * SIZE_OF_SQUARE + 1, j * SIZE_OF_SQUARE + 1, SIZE_OF_SQUARE - 1, SIZE_OF_SQUARE - 1, Qt::yellow);
            }
            if (pieces[i][j] == BLACK){
                p.setBrush(Qt::black);
            }
            if (pieces[i][j] == WHITE){
                p.setBrush(Qt::white);
            }
            if (pieces[i][j] != EMPTY){
                int x = i * SIZE_OF_SQUARE;
                int y = j * SIZE_OF_SQUARE;
                p.drawEllipse(x, y, PIECE_RADIUS*2, PIECE_RADIUS*2);
            }
        }
    }
}

void Board::clearLegalMoves(){
    for (int i = 0; i < SQUARE_PER_SIDE; i++){
        for (int j = 0; j < SQUARE_PER_SIDE; j++){
            legalMoves[i][j] = false;
        }
    }
}

bool Board::checkForPossibleMove(){
    clearLegalMoves();
    bool status = false;
    for (int i=0; i<SQUARE_PER_SIDE; i++){
        for (int j=0; j<SQUARE_PER_SIDE; j++){
            if(pieces[i][j]==EMPTY){
                QPoint current;
                current.setX(i*50);
                current.setY(j*50);
                if (isValidMove(current)){
                    legalMoves[i][j] = true;
                    status = true;
                }
            }
        }
    }
    return status;
}

bool Board::checkFirst(int x, int y){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    if(x>=0 && x<=9 && y>=0 && y<=9){
        if(pieces[x][y]!=EMPTY){
            if (pieces[x][y] == piece){
                return false;
            } else return true;
        } else return false;
    }else return false;
}

bool Board::makeMove(QPoint p) {
    bool check = false;
    p = translateToBoard(p);
    int x = p.x();
    int y = p.y();
    if ((x >= 0 && x <= 9) && (y >= 0 && y <= 9)) {
        if (pieces[x][y] ==  EMPTY){
            if (player  ==  black){
                pieces[x][y] = BLACK;
            } else {
                pieces[x][y] = WHITE;
            }
            check = true;
        }
    }
    return check;
}

QPoint Board::translateToBoard(QPoint p){

    int x = p.x()/50;
    int y = p.y()/50;
    QPoint point;
    point.setX(x);
    point.setY(y);
    return point;

}

void Board::playerSwitch(){
    if (player == white) {
        player = black;
    } else player = white;
}

void Board::flipPiece(QPoint p){

    QPoint temp = p;
    p=translateToBoard(p);
    if(pieces[p.x()][p.y()]==EMPTY){
        p = temp;

        if (checkDiagLowerLeft(p)){
            p=translateToBoard(p);
            for(int i=p.x()-1,j=p.y()+1; i>CHECK_X&&j<CHECK_Y; i--,j++){
                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkDiagUpperLeft(p)){
            p=translateToBoard(p);
            for(int i=p.x()-1,j=p.y()-1; i>CHECK_X&&j>CHECK_Y; i--,j--){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkDiagLowerRight(p)){
            p=translateToBoard(p);
            for(int i=p.x()+1,j=p.y()+1; i<CHECK_X&&j<CHECK_Y; i++,j++){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkDiagUpperRight(p)){
            p=translateToBoard(p);
            for(int i=p.x()+1,j=p.y()-1; i<CHECK_X&&j>CHECK_Y; i++,j--){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkHorizontalRight(p)){
            p=translateToBoard(p);
            for(int i=p.x()+1,j=p.y(); i<CHECK_X; i++){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkHorizontalLeft(p)){
            p=translateToBoard(p);
            for(int i=p.x()-1,j=p.y(); i>CHECK_X; i--){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkVerticalUp(p)){
            p=translateToBoard(p);
            for(int i=p.x(),j=p.y()-1; j>CHECK_Y; j--){

                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }
            p = temp;
        }

        if (checkVerticalDown(p)){
            p=translateToBoard(p);

            for(int i=p.x(),j=p.y()+1; j<CHECK_Y; j++){
                if (pieces[i][j]!=EMPTY){
                    if (player == black){
                        pieces[i][j] = BLACK;
                    } else pieces[i][j] = WHITE;
                }
            }p = temp;
        }
    }
}


bool Board::isValidMove(QPoint p){
    if(checkDiagLowerLeft(p)||checkDiagUpperLeft(p)||
            checkDiagLowerRight(p)||checkDiagUpperRight(p)||
            checkHorizontalLeft(p)||checkHorizontalRight(p)||
            checkVerticalUp(p)||checkVerticalDown(p)){
        return true;
    }else return false;
}

bool Board::checkDiagUpperLeft(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;

    if(checkFirst(p.x()-1,p.y()-1)){
        for(int i=p.x()-2,j=p.y()-2; (i>=0)&&(j>=0); i--,j--){
            if(pieces[i][j] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][j] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkDiagLowerLeft(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;

    if (checkFirst(p.x()-1,p.y()+1)) {
        for(int i=p.x()-2,j=p.y()+2; (i>=0)&&(j<=7); i--,j++){

            if(pieces[i][j] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][j] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkDiagUpperRight(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;

    if (checkFirst(p.x()+1,p.y()-1)){
        for(int i=p.x()+2,j=p.y()-2; (i<=9)&&(j>=0); i++,j--){
            if(pieces[i][j] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][j] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkDiagLowerRight(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;
    if (checkFirst(p.x()+1,p.y()+1)){
        for(int i=p.x()+2,j=p.y()+2; (i<=9)&&(j<=9); i++,j++){
            if(pieces[i][j] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][j] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkHorizontalLeft(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;
    if (checkFirst(p.x()-1,p.y())){
        for(int i=p.x()-2; (i>=0); i--){
            if (pieces[i][p.y()] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][p.y()] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=p.y();
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkHorizontalRight(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;
    if (checkFirst(p.x()+1,p.y())){
        for(int i=p.x()+2; (i<=9); i++){
            if (pieces[i][p.y()] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[i][p.y()] == piece){
                    check = true;
                    CHECK_X=i;
                    CHECK_Y=p.y();
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkVerticalUp(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p=translateToBoard(p);
    bool check = false;
    if (checkFirst(p.x(),p.y()-1)){
        for(int j=p.y()-2; j>=0; j--){

            if(pieces[p.x()][j] == EMPTY) {check=false; break;}
            else {
                if(pieces[p.x()][j] == piece){
                    check = true;
                    CHECK_X=p.x();
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

bool Board::checkVerticalDown(QPoint p){
    Piece piece;
    if (player == black){
        piece = BLACK;
    } else piece = WHITE;
    p = translateToBoard(p);
    bool check = false;
    if (checkFirst(p.x(),p.y()+1)){
        for(int j=p.y()+2; j<=9; j++){
            if(pieces[p.x()][j] == EMPTY) {
                check = false; break;
            }
            else {
                if(pieces[p.x()][j] == piece){
                    check = true;
                    CHECK_X=p.x();
                    CHECK_Y=j;
                    break;
                }
            }
        }
    }
    return check;
}

void Board::mousePressEvent(QMouseEvent * event)
{
    if (state == BEFORE_PLAY){
        reset();
    }

    if (state != PLAYING)
        return;

    if (event->button() == Qt::LeftButton){
        QPoint point = event->pos();
        if (isValidMove(point)){
            flipPiece(point);
            //update movelist
            int mx = point.x()/50;
            int my = point.y()/50;
            if (player==black)
                newmove = "Black Move: x ";
            else
                newmove = "White Move: x ";
            newmove += QString::number(mx);
            newmove += ", y ";
            newmove += QString::number(my);
            NewMoveAdded = true;

            movecount++;
            //Make the move and Store Boolean Value
            bool checkMoveMade = makeMove(point);

            if (checkMoveMade) {
                //Switch Player player
                playerSwitch();
            }
            //Execute only when there is no possible move for current player
            while (!checkForPossibleMove()){
                if (!lastPass){
                    lastPass = true;
                    playerSwitch();
                }else {
                    bothPass = true;
                    break;
                }
            }
        }
        //count the number of pieces
        count_black = 0;
        count_white = 0;
        for (int i = 0; i < SQUARE_PER_SIDE; i++){
            for (int j = 0; j < SQUARE_PER_SIDE; j++){
                if (pieces[i][j] == BLACK)
                    count_black++;
                if (pieces[i][j] == WHITE)
                    count_white++;
            }
        }
        if (bothPass || (count_black+count_white == 100)){
            state = AFTER_PLAY;
        }

    }
    update();
}
