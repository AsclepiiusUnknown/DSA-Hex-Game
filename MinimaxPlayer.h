/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "vector"

using namespace std;

class MinimaxPlayer : public Player
{
public:
    MinimaxPlayer(int t, string name = "Minimax") : // <--- add board here? is that possible??
            Player(t, name)
    {
    }

    struct Move
    {
        int x, y; //row and column for each move
    };

    bool getMove(Board *, int &, int &);

    bool isMovesLeft(int **grid, int boardSize);

    int evaluate(Board *board);

    bool evaluateLine(int playerType, Board *board);

    bool evaluateDFS(int playerType, Board *board);

    int minimax(Board *board, int depth, bool isMax);

    Move findBestMove(Board *board);
};

bool MinimaxPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    Move bestMove = findBestMove(board);
    x = bestMove.x;
    y = bestMove.y;

    return true;
}

int MinimaxPlayer::minimax(Board *board, int depth, bool isMax)
{
    // If there are no more moves and no winner then
    // it is a tie
    if (!isMovesLeft(board->getGrid(), board->getBoardSize()))
        return 0;

    int score = evaluate(board);
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType(), opponent = (getType() * -1);

    // If Maximizer has won, return its score
    if (score == 10)
        return score;

    // If Minimizer has won, return its score
    if (score == -10)
        return score;

    // If it's the maximizer's move
    if (isMax)
    {
        int best = -1000;

        // Traverse all cells
        for (int i = 0; i < bs; i++)
        {
            for (int j = 0; j < bs; j++)
            {
                // Check if cell is empty
                if (grid[i][j] == 0)
                {
                    // Make the move
                    grid[i][j] = player;

                    // Call minimax recursively and choose
                    // the maximum value
                    best = max(best,
                               minimax(board, depth + 1, !isMax));

                    // Undo the move
                    grid[i][j] = 0;
                }
            }
        }
        return best;
    }
        // If it's the minimizer's move
    else
    {
        int best = 1000;

        // Traverse all cells
        for (int i = 0; i < bs; i++)
        {
            for (int j = 0; j < bs; j++)
            {
                // Check if cell is empty
                if (grid[i][j] == 0)
                {
                    // Make the move
                    grid[i][j] = opponent;

                    // Call minimax recursively and choose
                    // the minimum value
                    best = min(best,
                               minimax(board, depth + 1, !isMax));

                    // Undo the move
                    grid[i][j] = 0;
                }
            }
        }
        return best;
    }
}

MinimaxPlayer::Move MinimaxPlayer::findBestMove(Board *board)
{
    int **grid = board->getGrid();
    int bestVal = -1000;
    int player = getType();
    Move bestMove;
    bestMove.x = -1;
    bestMove.y = -1;


    // Traverse all cells, evaluate minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // Check if cell is empty
            if (grid[i][j] == '_')
            {
                // Make the move
                grid[i][j] = player;

                // compute evaluation function for this
                // move.
                int moveVal = minimax(board, 0, false);

                // Undo the move
                grid[i][j] = '_';

                // If the value of the current move is
                // more than the best value, then update
                // best/
                if (moveVal > bestVal)
                {
                    bestMove.x = i;
                    bestMove.y = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    printf("The value of the best Move is : %d\n\n", bestVal);

    return bestMove;
}

bool MinimaxPlayer::isMovesLeft(int **grid, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (grid[i][j] == 0)
                return true;
    return false;
}

//region Heuristic Evaluation
int MinimaxPlayer::evaluate(Board *board)
{
    int spots = board->getSpotsSize();
    int bs = board->getBoardSize();

    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        int player = getType(), opponent = (getType() * -1);

        if (evaluateLine(player, board) || evaluateDFS(player, board))
            return player * 10;
        else if (evaluateLine(player, board) || evaluateDFS(player, board))
            return opponent * 10;
    }
    return 0;
}

bool MinimaxPlayer::evaluateLine(int playerType, Board *board)
{
    int bs = board->getBoardSize();
    int **grid = board->getGrid();

    //Check if a whole row is complete (for O)
    if (playerType == -1)
        for (int r = 0; r < bs; r++)
        {
            bool line = true;
            for (int c = 0; c < bs - 1; c++)
            {
                if (grid[r][c] != grid[r][c + 1] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    //Check if a whole column is complete (for X)
    if (playerType == 1)
        for (int c = 0; c < bs; c++)
        {
            bool line = true;
            for (int r = 0; r < bs - 1; r++)
            {
                if (grid[r][c] != grid[r + 1][c] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    return false;
}

bool MinimaxPlayer::evaluateDFS(int playerType, Board *board)
{
    int bs = board->getBoardSize();
    stack<int> trackStack;
    vector<int> visitedStack;

    //!optimise
    for (int i = 0; i < board->getSpotsSize(); i++)
    {
        trackStack.push(board->getSpots()[i]);
    }

    if (trackStack.empty())
        return false;

    bool start = false, finish = false;
    int startGoal = 0, endGoal = bs - 1;

    while (!trackStack.empty())
    {
        int s = trackStack.top();
        trackStack.pop();
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
        } else
        {
            stack<int> children = board->checkNeighbours(playerType, sX, sY);
            while (!children.empty())
            {
                if (find(visitedStack.begin(), visitedStack.end(), children.top()) !=
                    visitedStack.end())
                    children.pop();
                else
                {
                    trackStack.push(children.top());
                    children.pop();
                }
            }
        }
    }
    return false;
}
//endregion

#endif /* MINIMAX_H_ */
