#ifndef RANDOMPLAYER_H_
#define RANDOMPLAYER_H_

using namespace std;

class RandomPlayer : public Player
{
public:
    RandomPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Random AI") :
            Player(t, symbol, name)
    {
    }

    bool GetMove(Board *, int &, int &);
};

bool RandomPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random AI can't move cause the board is full!" << endl;
        return false;
    }

    vector<Cell> spots = board->getFreeCells();
    int index = rand() % (spots.size());

    x = spots[index].x;
    y = spots[index].y;

    //Check if our coordinate values are valid and log an error if not
    if (!board->validInput(x, y))
    {
        cout << "ERROR: Random AI input was invalid" << endl;
        return false;
    }

    return true;
}

#endif /* RANDOMPLAYER_H_ */
