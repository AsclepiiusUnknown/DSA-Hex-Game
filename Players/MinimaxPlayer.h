#include <random>
#include "../Cell.h"
#include "queue"
#include <unistd.h>

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
    MinimaxPlayer(int t, string symbol = "Undefined (ERROR)", int depth = 2500, string name = "Minimax") :
            Player(t, symbol, name), maxDepth(depth)
    {
    }

    const int MAX = round_toward_infinity;
    const int MIN = -round_toward_neg_infinity;
    const double WIN_VAL = 2.5; //250/100 (2.5) because 225 is the largest possible depth that can be simulated since we cap bs at 15 (15^2)

    int bs;
    int maxDepth = 2500;

    int player = type, opponent = type * -1;

    //region Functions
    bool GetMove(Board *, int &, int &);

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
    priority_queue<Move> moves;
    cout << "Values of Moves: " << endl;
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
                return {r, c, 1};
            }

            double value = Minimax(tempBoard, 0, false);

            Move m(r, c, value);
            moves.push(m);
            cout << setw(8) << value << endl;
        }
        cout << endl;
    }

    if (!moves.empty())
    {
        cout << moves.size() << "Best Value " << moves.top().v << endl;
        return moves.top();
    }

    cout << "ERROR: No appropriate move was found by Minimax" << endl;
    return {-1, -1, 0};
}

double MinimaxPlayer::Minimax(Board board, int depth, bool isMax)
{
    vector<Cell> emptyCells = board.getFreeCells();
    if (emptyCells.empty())
    {
        printf("FATAL ERROR: All cells used but no win was detected by Minimax!");
        return (board.Evaluation(-1, -1, player, opponent)) - depth;
    }

    //random_shuffle(emptyCells.begin(), emptyCells.end());
    double bestVal = (isMax) ? MIN : MAX;
    int playerType = (isMax) ? player : opponent;

    for (Cell i: emptyCells)
    {
        Board tempBoard(board);
        if (!tempBoard.AddTestMove(playerType, i.x, i.y))
            printf("ERROR: Invalid input created by Minimax's Max Move function\n");

        int status = tempBoard.Evaluation(i.x, i.y, player, opponent);

        //If this player has won
        if (status == player)
        {
            return WIN_VAL - depth;
        }
        else if (status == opponent)
        {
            return -WIN_VAL - depth;
        }
        else if (status != 0)
        {
            cout << "ERROR: Invalid evaluation status found in Minimax. Free Cells: " << tempBoard.freeCellsSize() << " Can Win: " << tempBoard.CanWin() << " Status: " << status << endl;
            return 0.0;
        }

        double thisVal = Minimax(tempBoard, depth + 0.01, !isMax);

        if ((isMax && thisVal > bestVal) || (!isMax && thisVal < bestVal))
            bestVal = thisVal;
    }
//    if ((bestVal == MAX || bestVal == MIN))
//        printf("\nERROR: BestUtil was never changed so no value was given to the move.");
    return bestVal;
}
//endregion
#endif /* MINIMAX_H_ */
// N 3 10 4 2 2