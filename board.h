#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QtGui>
#include <vector>

enum State { BEFORE_PLAY, PLAYING, PAUSED, AFTER_PLAY };

class Board : public QWidget
{
  Q_OBJECT

 public:
  Board(QWidget* parent);  
  State state;
  int count_black, count_white;
  Qt::GlobalColor player;
  bool NewMoveAdded;
  QString newmove;
  void reset(); // reset board

 protected:
  void mousePressEvent(QMouseEvent* event);
  void paintEvent(QPaintEvent * event);


 private:
  enum Piece { EMPTY, BLACK, WHITE } piece_t;



  // size of board
  static const int SQUARE_PER_SIDE = 10;

  // Array of pieces
  Piece pieces[SQUARE_PER_SIDE][SQUARE_PER_SIDE];

  bool legalMoves[SQUARE_PER_SIDE][SQUARE_PER_SIDE];

  static const int SIDE_LENGTH = 500;

  static const int SIZE_OF_SQUARE = SIDE_LENGTH / SQUARE_PER_SIDE;

  static const int PIECE_RADIUS = SIZE_OF_SQUARE/2;  

  int CHECK_X;
  int CHECK_Y;



  bool isValidMove(QPoint point);
  QPoint translateToBoard(QPoint point);
  void flipPiece(QPoint point);
  bool makeMove(QPoint point);
  void playerSwitch();
  bool checkForPossibleMove();
  bool checkFirst(int x, int y);
  bool checkDiagUpperLeft(QPoint p);
  bool checkDiagUpperRight(QPoint p);
  bool checkDiagLowerLeft(QPoint p);
  bool checkDiagLowerRight(QPoint p);
  bool checkHorizontalLeft(QPoint p);
  bool checkHorizontalRight(QPoint p);
  bool checkVerticalUp(QPoint p);
  bool checkVerticalDown(QPoint p);
  void clearLegalMoves();
};


#endif // BOARD_H
