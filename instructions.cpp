#include "instructions.h"
#include "ui_instructions.h"

Instructions::Instructions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instructions)
{
    ui->setupUi(this);
    setWindowTitle("Instructions");
    Rules = "Reversi Instruction: \n";
    Rules += "-This game can be played with 2 players.\n";
    Rules += "-All moves can only be made with the left mouse click.\n";
    Rules += "-The first player to go is black followed by white.\n";
    Rules += "-Possible moves for each player are highlighted in light green on their respective turn.\n";
    Rules += "-Each player alternate move until game end. If a player can not make a move he/she must pass.\n";
    Rules += "-Passing is handled automatically by the game's logic.\n";
    Rules += "-The game ends when there are no more squares on the board to be filled or if both player pass their turn consecutively.\n";
    Rules += "-The score will be calculated automatically and display in the score panel below the board.\n";
    Rules += "-The winner has the higher score. The game is a draw if both player has the same score.\n";
    ui->textInstructions->setText(Rules);
    ui->textInstructions->setFontWeight(14);

}

Instructions::~Instructions()
{
    delete ui;
}
