/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MONTECARLOPLAYER_H_
#define MONTECARLOPLAYER_H_

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include "vector"
#include "queue"
#include "time.h"

using namespace std;

class MonteCarloPlayer : public Player
{
public:
    MonteCarloPlayer(int t, string name = "Monte Carlo") :
            Player(t, name)
    {
    }

    struct Move
    {
        int x, y; //coordinates for the move
        int v; //Move value

        Move(int _x, int _y, double _v) : x(_x), y(_y), v(_v)
        {}

        bool operator<(const Move &m) const
        {
            return v < m.v;
        };
    };

    const int WIN_VAL = 10;

    int player = getType(), opponent = (getType() * -1);

    bool getMove(Board *, int &, int &);

    double simulation(Board board);

    double expansion(int playerType, Board board);

    int evaluate(Board &lB);

    bool evaluateDFS(int playerType, Board &lB);

    void getRandomMove(vector<int> spots, int &x, int &y, int bs);
};

bool MonteCarloPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    int player = getType();

    priority_queue<Move> moves;
    cout << "Values of Moves: " << endl;
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            if (board->grid[r][c] != 0)
            {
                cout << "  x   ";
                continue;
            }
            Board tempBoard = *board;

            tempBoard.addMove(player, r, c);
            if (tempBoard.checkWinningStatus(player, r, c))
            {
                x = r;
                y = c;
                return true;
            }

            double utility = simulation(tempBoard);

            Move m(r, c, utility);
            moves.push(m);
            cout << setw(5) << m.v << " ";
        }
        cout << endl;
    }

    if (!moves.empty())
    {
        x = moves.top().x;
        y = moves.top().y;
        cout << "Best value " << moves.top().v << endl;
    }

    return true;
}

double MonteCarloPlayer::simulation(Board board)
{
    int times = 10000;
    double winning = 0.0;

    for (int i = 0; i < times; i++)
    {
        Board tempBoard(board);
        winning += expansion(opponent, tempBoard);
    }
    return (winning / (double) times);
}

double MonteCarloPlayer::expansion(int playerType, Board board)
{
    if (evaluate(board) == (playerType * WIN_VAL))
    {
        return 1.0;
    } else
    {
        return 0.0;
    }


    int x, y;
    getRandomMove(board.getFSpots(), x, y, board.getBoardSize());
    board.addMove(player, x, y);
    playerType = (playerType == -1) ? 'O' : 'X';

    return expansion(player, board);
}

void MonteCarloPlayer::getRandomMove(vector<int> spots, int &x, int &y, int bs)
{
    srand(time(NULL));

    int i = rand() % spots.size();
    x = i / bs;
    y = i % bs;
}

//region Heuristic Evaluation
int MonteCarloPlayer::evaluate(Board &lB)
{
    int spots = lB.FSpotsSize();
    int bs = lB.getBoardSize();

    //Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        //Check both players for a winner, first checking for a line, then using DFS
        if (lB.lineWin(player) || evaluateDFS(player, lB))
            return (player * WIN_VAL);
    }
    return 0;
}

bool MonteCarloPlayer::evaluateDFS(int playerType, Board &lB)
{
    stack<int> searchStack;
    vector<int> visitedStack;
    int bs = lB.getBoardSize();

    for (int i = 0; i < bs; i++)
        for (int j = 0; j < bs; j++)
            if (lB.grid[i][j] == playerType)
                searchStack.push(lB.grid[i][j]);

    if (searchStack.empty())
        return false;

    bool start = false, finish = false;
    int startGoal = 0, endGoal = bs - 1;

    while (!searchStack.empty())
    {
        int s = searchStack.top();
        searchStack.pop();
        visitedStack.push_back(s);

        int sX = s / bs;
        int sY = s % bs;

        if (playerType == -1)
        {
            if (sY == startGoal)
                start = true;
            else if (sY == endGoal)
                finish = true;
        } else if (playerType == 1)
        {
            if (sX == startGoal)
                start = true;
            else if (sX == endGoal)
                finish = true;
        }

        if (start && finish)
        {
            return true;
        }
    }

    return false;
}
//endregion

#endif /* MONTECARLOPLAYER_H_ */
