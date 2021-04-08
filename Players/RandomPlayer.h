/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef RANDOMPLAYER_H_
#define RANDOMPLAYER_H_

#include<time.h>
#include<random>
#include<vector>

using namespace std;

class RandomPlayer : public Player
{
public:
    RandomPlayer(int t, string name = "Random") :
            Player(t, name)
    {
    }

    bool getMove(Board *, int &, int &);
};

bool RandomPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    srand(time(NULL));
    vector<int> spots = board->getFreeCells();
    int index = rand() % (spots.size());

    x = spots[index] / board->getBoardSize();
    y = spots[index] % board->getBoardSize();

    return true;
}

#endif /* RANDOMPLAYER_H_ */
