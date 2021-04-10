#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

using namespace std;

class HumanPlayer : public Player
{
public:
    HumanPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Human") :
            Player(t, symbol, name)
    {
    }

    bool GetMove(Board *, int &, int &);
};

bool HumanPlayer::GetMove(Board *board, int &x, int &y)
{
    bool flag = false;
    bool lastTurn = false;
    int bs = board->getBoardSize();

    lastTurn = board->isFullThisTurn();

    do
    {
        cout << "Input row and column (x, y) between 1 to " << bs << " for " << name << ":";
        board->PrintCells(board->getFreeCells());
        int row, col;
        cin >> row >> col;
        x = row - 1;
        y = col - 1;
        flag = !board->validInput(x, y); //Flag invalid input if given coordinates are invalid
        if (flag)
            cout << "Invalid input! Please input again." << endl;
    } while (flag);

    //system("CLS");
    return true;
}

#endif /* HUMANPLAYER_H_ */
