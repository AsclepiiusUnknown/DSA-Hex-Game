/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MINIMAX_H_
#define MINIMAX_H_

#include<time.h>
#include<random>
#include<vector>

using namespace std;

class MiniMaxPlayer : public Player
{
public:
    MiniMaxPlayer(int t, string name = "Monte Carlo") :
            Player(t, name)
    {
    }

    bool getMove(Board *, int &, int &);

    int minimax(int **grid, int player, int boardSize);
};

bool MiniMaxPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    int **grid = board->getGrid();
    int boardSize = board->getBoardSize();

    int move = -1;
    int score = -2;

    for (int r = 0; r < boardSize; ++r)
        for (int c = 0; c < boardSize; c++)
        {
            if (grid[r][c] == 0)
            {
                grid[r][c] = 1;
                int tempScore = -minimax(grid, getType(), boardSize);
                grid[r][c] = 0;

                if (tempScore > score)
                {
                    score = tempScore;
                    move = r * boardSize + c;
                }
            }
        }

    x = move / boardSize;
    y = move % boardSize;

    return true;
}

int MiniMaxPlayer::minimax(int **grid, int player, int boardSize)
{
    int move = -1;
    int score = -2;

    for (int r = 0; r < boardSize; ++r)
        for (int c = 0; c < boardSize; c++)
        {
            if (grid[r][c] == 0)
            {
                grid[r][c] = player;
                int thisScore = -minimax(grid, player * -1, boardSize);

                if (thisScore > score)
                {
                    score = thisScore;
                    move = r * boardSize + c;
                }
                grid[r][c] = 0;//Reset board after try
            }
        }
    if (move == -1)
        return 0;

    return score;
}

#endif /* MINIMAX_H_ */
