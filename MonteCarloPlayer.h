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
#include <string>
#include "vector"
#include "queue"

using namespace std;

class MonteCarloPlayer : public Player
{
public:
    MonteCarloPlayer(int t, string name = "Monte Carlo") :
            Player(t, name)
    {
    }

    bool getMove(Board *, int &, int &);

    int next(Board *board, int playerType);

    double getWins(Board *board, int playerType);

    bool place(int x, int y, int playerType, int **grid);

    bool badMove(int x, int y, int **grid);

    bool inBoard(int x, int y, int bs);

    int winner(Board *board);

    void bfsSearch(vector<int> &start, vector<bool> &condition, Board *board);

    void borders(int x, int y, vector<bool> &condition, char side, int bs);

    static int direct[6][2];
};

int MonteCarloPlayer::direct[6][2] =
        {
                {-1, 0},
                {-1, 1},
                {0,  -1},
                {0,  1},
                {1,  -1},
                {1,  0} // corners
        };

bool MonteCarloPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    int bestMove = next(board, getType());

    x = bestMove / board->getBoardSize();
    y = bestMove % board->getBoardSize();

    return true;
}


// montecarlo simulation, with getWins() it finds the
// value of moves by making random permutations and doing simulation moves
// on each and tracks no. wins. The moves are given the no.wins as a move
// value, the best value is the best move.
int MonteCarloPlayer::next(Board *board, int playerType)
{
    vector<int> blank = board->getFSpots();
    double bestMove = 0;
    int move = blank[0];
    int bs = board->getBoardSize();

    for (int i = 0; i < blank.size(); i++)
    {
        int x = blank[i] / bs;
        int y = blank[i] % bs;
        place(x, y, playerType, board->getGrid());

        double moveValue = getWins(board, playerType);
        if (moveValue > bestMove)
        {
            move = blank[i];
            bestMove = moveValue;
        }

        badMove(x, y, nullptr);
    }
    return move;
}

// helper for next()
// all  empty spots on board stored on  permutation
// then rand. empty spot picked and played off of,
// wins are tracked and the win value is then returned.
double MonteCarloPlayer::getWins(Board *board, int playerType)
{
    int bs = board->getBoardSize();
    auto blank = board->getFSpots();
    int winCount = 0;
    vector<int> perm(blank.size());
    for (int i = 0; i < perm.size(); i++)
        perm[i] = i;
    for (int n = 0; n < 1000; n++)
    {
        int turn = playerType;
        for (int i = perm.size(); i > 1; i--)
        {
            int swap = rand() % i;
            int temp = perm[i - 1];
            perm[i - 1] = perm[swap];
            perm[swap] = temp; // prand the permutation
        }
        for (int i = 0; i < perm.size(); i++)
        {
            turn = !turn; //easy bool turn tracking
            int x = blank[perm[i]] / bs;
            int y = blank[perm[i]] % bs;
            if (turn)
            {
                place(x, y, 1, nullptr);
            } else
            {
                place(x, y, 2, nullptr);
            }
        }
        if (winner(board) == playerType)
            winCount++;

        for (int i : blank)
            badMove(i / bs, i % bs, nullptr); // take back rand moves
    }
    return static_cast<double>(winCount) / 1000;
}

// "places" a piece on the board, assigns cell block to B or W
bool MonteCarloPlayer::place(int x, int y, int playerType, int **grid)
{
    if (inBoard(x, y, 0) && grid[x][y] == 0)
    {
        if (playerType == getType())
            grid[x][y] = playerType;
        else
            grid[x][y] = playerType * -1;
        return true;
    }
    return false;
}

// retracts a badMove (or in my case the random suboptimals)
bool MonteCarloPlayer::badMove(int x, int y, int **grid)
{
    if (inBoard(x, y, 0))
    {
        grid[x][y] = 0;
        return true;
    }
    return false;
}

// checks if placed piece is a legal and in bounds
bool MonteCarloPlayer::inBoard(int x, int y, int bs)
{
    return (x < bs && y < bs && x >= 0 && y >= 0);
}

// there logically must be one winner so when win condition met use
// BFS to check if black made a path from left to right, if not
// white is necessarily the winner
int MonteCarloPlayer::winner(Board *board)
{
    int bs = board->getBoardSize();
    int **grid = board->getGrid();
    int playerType = getType();

    vector<bool> condition(2, false); // tracks side to side win
    vector<int> start;
    for (int i = 0; i < bs; i++)
        if (grid[i][0] == playerType)
            start.push_back(i * bs);

    bfsSearch(start, condition, board);
    return (condition[0] && condition[1]) ? playerType : (playerType * -1);
}

// Breadth-first search and checks borders() win conditionals
void MonteCarloPlayer::bfsSearch(vector<int> &start, vector<bool> &condition, Board *board)
{
    if (start.size() != 0)
    {
        int bs = board->getBoardSize();
        int **grid = board->getGrid();

        int x = start[0] / bs;
        int y = start[0] % bs;
        char side = grid[x][y];

        vector<vector<bool>> visited(bs, vector<bool>(bs));
        queue<int> trace;


        for (int &i : start)
        {
            trace.push(i);
            visited[i / bs][i % bs] = true;
        }
        while (!(trace.empty()))
        {
            int top = trace.front();
            int topX = top / bs, topY = top % bs;
            borders(topX, topY, condition, side, bs);
            trace.pop();

            for (int i = 0; i < 6; i++)
            {
                int xCursor = topX + direct[i][0];
                int yCursor = topY + direct[i][1];
                if (inBoard(xCursor, yCursor, bs) && grid[xCursor][yCursor] == side && !visited[xCursor][yCursor])
                {
                    visited[xCursor][yCursor] = true;
                    trace.push(xCursor * bs + yCursor);
                }
            }
        }
    }
}

// helper for win(), mutates conditionals (2 element vector) to check borders
void MonteCarloPlayer::borders(int x, int y, vector<bool> &condition, char side, int bs)
{
    if (side == -1)
    {
        if (y == 0)
            condition[0] = true;
        if (y == bs - 1)
            condition[1] = true;

    } else
    {
        if (x == 0)
            condition[0] = true;
        if (x == bs - 1)
            condition[1] = true;
    }
}

#endif /* MONTECARLOPLAYER_H_ */
