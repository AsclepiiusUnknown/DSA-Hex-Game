#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

using namespace std;

class HumanPlayer : public Player
{
public:
    HumanPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Human") : Player(t, symbol, name)
    {}

    bool GetMove(Board *, int &, int &);
};

bool HumanPlayer::GetMove(Board *board, int &x, int &y)
{
    bool flag = false;
    bool lastTurn = false;
    int bs = board->GetBoardSize();

    do
    {
        cout << "Input row and column (x, y) between 1 to " << bs << " for " << name << ":";
        board->PrintCells(board->GetEmpty());
        int row, col;
        cin >> row >> col;
        x = row - 1;
        y = col - 1;
        flag = !board->IsValidInput(x, y);
        if (flag)
            cout << "Invalid input! Please input again." << endl;
    } while (flag);

    return true;
}

#endif /* HUMANPLAYER_H_ */
