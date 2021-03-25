/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MONTECARLOPLAYER_H_
#define MONTECARLOPLAYER_H_

#include<time.h>
#include<random>
#include<vector>

using namespace std;

class MonteCarloPlayer : public Player
{
public:
    MonteCarloPlayer(int t, string name = "Monte Carlo") :
            Player(t, name)
    {
    }

    bool getMove(Board *, int &, int &);
};

bool MonteCarloPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    srand(time(NULL));
    vector<int> spots = board->getSpots();
    int index = rand() % (spots.size());

    x = spots[index] / board->getBoardSize();
    y = spots[index] % board->getBoardSize();

    return true;
}

#endif /* MONTECARLOPLAYER_H_ */
