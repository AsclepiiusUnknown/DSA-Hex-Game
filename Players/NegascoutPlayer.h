//region Definitions
/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef NEGASCOUT_H_
#define NEGASCOUT_H_

#include "../Cell.h"
//endregion

class NegascoutPlayer : public Player
{
public:
    NegascoutPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Negascout") :
            Player(t, symbol, name)
    {
    }

    const int MAX = 1000;
    const int MIN = -1000;
    const int WIN_VAL = 10;

    //region Functions
    bool GetMove(Board *, int &, int &);

    bool isMovesLeft(int **grid, int boardSize);

    int evaluate(Board *board);

    bool evaluateDFS(Board *board, int playerType);

    Move BNS(Board *board, int A, int B, Cell node, int playerType);

    Move BestMove(Board *board);

    bool isInVector(vector<Cell> v, Cell e);

    //endregion
    int NextGuess(int A, int B, int subCount);

    int factorial(int n);
};

bool NegascoutPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    //Start the algorithm to get the best move
    Move bestMove = BestMove(board);

    //Assign the coordinates of the best move
    x = bestMove.x;
    y = bestMove.y;

    if (!board->validInput(x, y))
    {
        cout << "ERROR: Minimax input was invalid" << endl;
        return false;
    }

    return true;
}


int NegascoutPlayer::NextGuess(int A, int B, int subCount)
{
    return A + (B - A) * (subCount - 1) / subCount;
}

//region Minimax Algorithm
Move NegascoutPlayer::BestMove(Board *board)
{
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType();
    int bestVal = (player == -1) ? MIN : MAX;
    bool isMax = (player == 1);
    Move bestMove(-1, -1, 0);

    //Traverse all free cells
    for (Cell i : board->getFreeCells())
    {
        //Start by making the cell ours
        grid[i.x][i.y] = player;

        //Evaluate the move's value using BNS algorithm & alpha-beta pruning
        Move m = BNS(board, MAX, MIN, i, player);

        //Reset the cell to unoccupied
        grid[i.x][i.y] = 0;

        //If the move being tested is our current best
        if (m.v > bestVal)
        {
            //Set to best values
            bestMove.x = m.x;
            bestMove.y = m.y;
            bestVal = m.v;
        }
    }

//Clear console then print the value of the best move
    system("CLS");
    cout << "The value of the best Cell is : " << bestVal <<
         endl;

    return
            bestMove;
}

Move NegascoutPlayer::BNS(Board *board, int A, int B, Cell node, int playerType)
{
    board->AddTestMove(playerType, node.x, node.y);
    vector<Cell> subs = board->getFreeCells();
    int betterCount = 0;
    Move bestNode(-1, -1, 0);

    do
    {
        int test = NextGuess(A, B, factorial(subs.size()));
        betterCount = 0;
        for (Cell i : subs)
        {
            int bestValue = 0;//-AB(i, -test, -(test - 1));
            if (bestValue >= test)
            {
                betterCount = betterCount + 1;
                bestNode.x = i.x;
                bestNode.y = i.y;
                bestNode.v = bestValue;
            }
        }


    } while (!((B - A) < 2 || betterCount == 1));
    return bestNode;
}

int CountChildren(int n)
{
    if (n == 0)
        return 0;

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        count += CountChildren(n - 1);
    }

    return count;
}

int NegascoutPlayer::factorial(int n)
{
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}
//endregion
#endif /* MINIMAX_H_ */
