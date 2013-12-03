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
    setFixedSize(SIDE_LENGTH, SIDE_LENGTH);
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
    update();
}

void Board::paintEvent(QPaintEvent *){

    cout<<"Running painter\n";
    QPainter p(this);

    //Draw the board
    p.drawRect(0, 0, SIDE_LENGTH-1, SIDE_LENGTH-1);


    //Repaint the pieces each time
    for (int i = 0; i < SQUARE_PER_SIDE; i++){
        for (int j = 0; j < SQUARE_PER_SIDE; j++){
            p.drawRect(i * SIZE_OF_SQUARE,j * SIZE_OF_SQUARE, SIZE_OF_SQUARE, SIZE_OF_SQUARE);
            p.fillRect(i * SIZE_OF_SQUARE + 1, j * SIZE_OF_SQUARE + 1, SIZE_OF_SQUARE - 1, SIZE_OF_SQUARE - 1, Qt::green);
            if (legalMoves[i][j] == 1){
                p.fillRect(i * SIZE_OF_SQUARE + 1, j * SIZE_OF_SQUARE + 1, SIZE_OF_SQUARE - 1, SIZE_OF_SQUARE - 1, Qt::yellow);
            }
            if (pieces[i][j] == BLACK){
//                cout<< "Piece Type: " << pieces[i][j] << " @ index: " << i << " " << j << "\n";
                p.setBrush(Qt::black);
            }
            if (pieces[i][j] == WHITE){
//                cout<< "Piece Type: " << pieces[i][j] << " @ index: " << i << " " << j << "\n";
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
    //cout << " Check first x: " << x << endl;
    //cout << " Check first y: " << y << endl;
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
    //cout<< "X: " << x << endl;
    //cout << "Y: " << y << endl;
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
    if (state == AFTER_PLAY){
        state = BEFORE_PLAY;
        reset();
    }

    if (state != PLAYING)
        return;

    if (event->button() == Qt::LeftButton){
        cout << "Calling Mouse click listener\n";
        QPoint point = event->pos();
        //bool temp = isValidMove(point);
        //cout << temp << endl;
        if (isValidMove(point)){
            cout << "Is valid move!" << endl;
            /*while(isValidMove(point)){//Flip the pieces in all applicable direction
                QPoint p = translateToBoard(point);
                if (pieces[p.x()][p.y()] != EMPTY){
                    if (movecount==98 || bothPass){
                        //Display When User Attempt to Make Move When Game Ended
                    }
                    else {
                        //Display When User Attempt to Make Move On Non-Empty Space
                    }break;
                }
                flipPiece(point);
            }*/
            flipPiece(point);
            movecount++;
            bool checkMoveMade = makeMove(point);//Make the move and Store Boolean Value

            if (checkMoveMade) {
                playerSwitch();//Switch Player player
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
        if (bothPass || movecount == 96){
            state = AFTER_PLAY;
            cout << "finished!" << endl;
            // Add code for Game Ending
        }

    }
    update();
}
