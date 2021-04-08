//
// Created by ascle on 8/04/2021.
//

#ifndef DSA_HEX_GAME_MINIMAXPLAYER2_H
#define DSA_HEX_GAME_MINIMAXPLAYER2_H

#include<iostream>
#include<sstream>
#include<limits>
#include "../Cell.h"

using namespace std;

class MinimaxPlayer2 : public Player
{
public:
    MinimaxPlayer2(int t, string name = "Minimax") :
            Player(t, name)
    {
    }

    bool getMove(Board, int &, int &);


    double maxMove(Board board);

    double minMove(Board board);

    int steps{};
};

bool MinimaxPlayer2::getMove(Board board, int &x, int &y)
{

    vector<int> emptyCells = board.getFreeCells();
    if (emptyCells.empty())
        return false;
    random_shuffle(emptyCells.begin(), emptyCells.end());


    priority_queue<Cell> moves;

    for (int index : emptyCells)
    {
        steps = 100000;
        int i = index / board.getBoardSize();
        int j = index % board.getBoardSize();
        Board tempBoard(board);
        tempBoard.addMove(getType(), i, j);
        if (tempBoard.isBoardFull())
        {
            if (tempBoard.checkWinningStatus(getType(), i, j))
            {
                x = i;
                y = j;
                return true;
            }
            else
            {
                continue;
            }
        }
        double minUtility = minMove(tempBoard);
        Cell c(i, j, minUtility);
        moves.push(c);
    }

    if (moves.size() > 0)
    {
        x = moves.top().x;
        y = moves.top().y;

        cout << moves.size() << " MiniMax Utility = " << moves.top().heuristic
             << " at (" << (x + 1) << "," << (y + 1) << ")" << endl;
    }

    return false;
}

double MinimaxPlayer2::maxMove(Board board)
{
    vector<int> emptyCells = board.getFreeCells();
    if (emptyCells.empty() || steps <= 0)
        return 0.01;

    random_shuffle(emptyCells.begin(), emptyCells.end());

    double maxUtility = -2;
    for (int index : emptyCells)
    {
        int i = index / board.getBoardSize();
        int j = index % board.getBoardSize();
        Board tempBoard(board);
        tempBoard.addMove(getType(), i, j);
        if (tempBoard.freeCellsSize() <= 0)
        {
            if (tempBoard.checkWinningStatus(getType(), i, j))
                return 1; //score 1 if wins
            else if (tempBoard.checkWinningStatus(getType() * -1, i, j))
                return -1; //score -1 if lose
            else
                return 0; //score 0 if tie
        }

        double utility;
        utility = minMove(tempBoard);

        if (utility > maxUtility)
        {
            maxUtility = utility;
        }
    }
    steps--;
    return maxUtility;
}

double MinimaxPlayer2::minMove(Board board)
{

    vector<int> emptyCells = board.getFreeCells();
    if (emptyCells.empty() || steps <= 0)
        return -0.01;

    random_shuffle(emptyCells.begin(), emptyCells.end());
    double minUtility = 2;
    for (int index : emptyCells)
    {
        int i = index / board.getBoardSize();
        int j = index % board.getBoardSize();
        Board tempBoard(board);

        tempBoard.addMove(getType(), i, j);
        if (tempBoard.freeCellsSize() <= 0)
        {
            if (tempBoard.checkWinningStatus(getType(), i, j))
                return 1; //score 1 if wins
            else if (tempBoard.checkWinningStatus(getType() * -1, i, j))
                return -1; //score -1 if lose
            else
                return 0; //score 0 if tie
        }

        double utility = maxMove(board);

        if (utility < minUtility)
        {
            minUtility = utility;
        }
    }
    steps--;
    return minUtility;
}

#endif //DSA_HEX_GAME_MINIMAXPLAYER2_H
