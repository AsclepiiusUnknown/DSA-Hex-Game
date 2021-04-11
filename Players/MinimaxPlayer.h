#include <random>
#include "../Cell.h"
#include "queue"

//region Definitions
/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MINIMAX_H_
#define MINIMAX_H_
//endregion

class MinimaxPlayer : public Player
{
public:
    MinimaxPlayer(int t, string symbol = "Undefined (ERROR)", double depth = 12, string name = "Minimax") :
            Player(t, symbol, name), maxDepth(depth)
    {
    }

    const int MAX = 1000;
    const int MIN = -1000;
    const int WIN_VAL = 1000;

    int bs;
    double maxDepth;

    int player = type, opponent = type * -1;

    //region Functions
    bool GetMove(Board *, int &, int &);

    int Evaluate(Board board, int x, int y);

    Move BestMove(Board *board);

    //endregion
    double Minimax(Board board, int depth, bool isMax);
};

bool MinimaxPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    //Setup the global bs (board size) variable
    bs = board->getBoardSize();

    Move m = BestMove(board);
    x = m.x;
    y = m.y;

    //Check if our coordinate values are valid and log an error if not
    if (!board->validInput(x, y))
    {
        cout << "ERROR: Minimax input was invalid" << endl;
        return false;
    }

    return true;
}

//region Minimax Algorithm
Move MinimaxPlayer::BestMove(Board *board)
{
    //todo random_shuffle(freeCells.begin(), freeCells.end());

    priority_queue<Move> moves;
    for (int r = 0; r < bs; r++)
    {
        for (int c = 0; c < bs; c++)
        {
            board->printCoord(r + 1, c + 1, false);

            if (board->grid[r][c] == 1)
            {
                cout << setw(8) << "X" << endl;
                continue;
            }
            else if (board->grid[r][c] == -1)
            {
                cout << setw(8) << "O" << endl;
                continue;
            }

            Board tempBoard(*board);
            tempBoard.AddTestMove(player, r, c);

            if (tempBoard.CheckForWin(player, r, c)) //if the board is full
            {
                printf("Winning move found!");
                return {r, c, 0};
            }

            double value = Minimax(tempBoard, maxDepth, false);

            Move m(r, c, value);
            moves.push(m);
            cout << setw(8) << value << endl;
        }
        cout << endl;
    }

    if (!moves.empty())
    {
        cout << moves.size() << "Best Value " << moves.top().v << " at (" << (moves.top().x + 1) << "," << (moves.top().y + 1) << ")" << endl;
        return moves.top();
    }

    cout << "ERROR: No appropriate move was found by Minimax" << endl;
    return Move(-1, -1, 0);
}

double MinimaxPlayer::Minimax(Board board, int depth, bool isMax)
{
    vector<Cell> emptyCells = board.getFreeCells();
    if (depth <= 0 || emptyCells.empty())
    {
        return -0.01;
        printf("not R??");
    }

    random_shuffle(emptyCells.begin(), emptyCells.end());
    double bestUtil = (isMax) ? MIN : MAX;
    for (Cell i : emptyCells)
    {
        int x = i.x, y = i.y;
        Board tempBoard(board);
        if (isMax)
        {
            if (!tempBoard.AddTestMove(player, x, y)) //todo  opponent or dynamic player variable?
                printf("ERROR: Invalid input created by Minimax's Max Move function\n");
        }
        else
        {
            if (!tempBoard.AddTestMove(opponent, x, y)) //todo  opponent or dynamic player variable?
                printf("ERROR: Invalid input created by Minimax's Max Move function\n");
        }
        int status = Evaluate(board, 0, 0);
        //If this player is winning
        if (status == player)
        {
            return (double) WIN_VAL - depth;
        }
        else if (status == opponent)
        {
            return (double) -WIN_VAL - depth;
        }
        else if (status != 0) //Continue if we aren't finished and no winner
            return 0.0;

        double utility = Minimax(board, depth - 1, !isMax);

        if ((isMax && utility > bestUtil) || (!isMax && utility < bestUtil))
        {
            bestUtil = utility;
        }
    }
    return bestUtil;
}
//endregion

//region Heuristic Evaluation
int MinimaxPlayer::Evaluate(Board board, int x, int y)
{
    int freeCount = board.freeCellsSize();

    //Only check for a win if enough cells have been occupied
    if ((freeCount + (bs * 2 - 1)) <= (bs * bs))
    {
        //Check both players for a winner, first checking for a line, then using DFS
        if (board.CheckForWin(player, x, y))
            return (player);
        if (board.CheckForWin(opponent, x, y))
            return (opponent);
    }

    if (freeCount > 0)
        return 0; //continue value

    return 5; //Error Check (shouldn't reach this point)
}
//endregion
#endif /* MINIMAX_H_ */