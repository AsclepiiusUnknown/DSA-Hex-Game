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
    MinimaxPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Minimax") :
            Player(t, symbol, name)
    {
    }

    struct Move
    {
        int x, y; //coordinates for the move
        double v; //Cell value

        Move(int _x, int _y, double _v) : x(_x), y(_y), v(_v)
        {}

        bool operator<(const Move &m) const
        {
            return v < m.v;
        };
    };

    const int MAX = 1000;
    const int MIN = -1000;
    const int WIN_VAL = 50;
    const int MAX_DEPTH = 100;

    int bs;

    int player = type, opponent = type * -1;

    //region Functions
    bool GetMove(Board *, int &, int &);

    bool isMovesLeft(int **grid);

    int evaluate(Board &lB);

    bool evaluateDFS(int playerType, Board &lB);

    int minimax(Board &lB, int depth, bool isMax, int alpha, int beta);

    MinimaxPlayer::Move BestMove(Board *board);

    //endregion
    double MinMove(Board board, double depth);

    double MaxMove(Board board, double depth);
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
MinimaxPlayer::Move MinimaxPlayer::BestMove(Board *board)
{

    vector<Cell> freeCells = board->getFreeCells();
    //todo random_shuffle(freeCells.begin(), freeCells.end());

    priority_queue<Move> moves;
    for (Cell i : freeCells)
    {
        int x = i.x, y = i.y;
        Board tempBoard(*board);
        tempBoard.grid[x][y];
        tempBoard.removeFreeCell(x, y);
        if (tempBoard.freeCellsSize() == 0) //if the board is full
        {
            if (evaluateDFS(player, tempBoard))
            {
                return Move(x, y, 0);
            }
            else
                continue;
        }

        double min = MinMove(tempBoard, 0);
        Move m(x, y, min);
        moves.push(m);

        cout << "(" << (moves.top().x + 1) << "," << (moves.top().y + 1) << ")" << setw(4) << moves.top().v << endl;
    }

    if (moves.size() > 0)
    {
        cout << moves.size() << " MiniMax Utility = " << moves.top().v << " at (" << (moves.top().x + 1) << "," << (moves.top().y + 1) << ")" << endl;
        return moves.top();
    }

    cout << "ERROR: Minimax was unable to find a suitable move" << endl;
    return Move(-1, -1, 0);
}

double MinimaxPlayer::MinMove(Board board, double depth)
{
    vector<Cell> emptyCells = board.getFreeCells();
    if (emptyCells.empty() || depth > MAX_DEPTH)
        return -0.01;

    random_shuffle(emptyCells.begin(), emptyCells.end());
    double minUtility = MAX;
    for (Cell i : emptyCells)
    {
        int x = i.x, y = i.y;
        Board tempBoard(board);
        tempBoard.grid[x][y] = -1;
        tempBoard.removeFreeCell(x, y);

        int status = evaluate(board);
        //If this player is winning
        if (status == player)
        {
            return WIN_VAL - depth;
        }
        else if (status == opponent)
        {
            return -WIN_VAL - depth;
        }
        else if (status != 0) //Continue if we aren't finished and no winner
            return 0.0;

        double utility = MaxMove(board, depth++);

        if (utility < minUtility)
        {
            minUtility = utility;
        }
    }
    return minUtility;
}

double MinimaxPlayer::MaxMove(Board board, double depth)
{
    vector<Cell> emptyCells = board.getFreeCells();
    if (emptyCells.empty() || depth > MAX_DEPTH)
        return 0.01;

    random_shuffle(emptyCells.begin(), emptyCells.end());

    double maxUtility = MIN;
    for (Cell i : emptyCells)
    {
        int x = i.x, y = i.y;
        Board tempBoard(board);
        tempBoard.grid[x][y] = opponent; //todo  opponent or dynamic player variable?
        tempBoard.removeFreeCell(x, y);

        int status = evaluate(board);
        //If this player is winning
        if (status == player)
        {
            //todo  return 1.0 * player - depth;
            return WIN_VAL - depth;
        }
        else if (status == opponent)
        {
            return -WIN_VAL - depth;
        }
        else if (status != 0) //Continue if we aren't finished and no winner
            return 0.0;

        double utility;
        utility = MinMove(board, depth++);

        if (utility > maxUtility)
        {
            maxUtility = utility;
        }
    }
    return maxUtility;
}

/*int MinimaxPlayer::minimax(Board &lB, int depth, bool isMax, int alpha, int beta)
{
    int score = evaluate(lB);

    //If the board is full without a winner then there is a tie
    if (!isMovesLeft(lB.grid))
        return 0;

    //If Maximizer/Minimizer has won, return its score
    if (score == WIN_VAL || score == -WIN_VAL || depth <= 0)
        return score;

    //If it's the Maximizer's move
    if (isMax)
    {
        int best = MIN; //-1000

        // Traverse all cells
        for (int x = 0; x < bs; x++)
        {
            for (int y = 0; y < bs; y++)
            {
                // Check if cell is empty
                if (lB.grid[x][y] == 0)
                {
                    //Start by making the cell ours
                    lB.grid[x][y] = player;

                    //Recursively call algorithm and choose max value
                    best = max(best, minimax(lB, depth - 1, !isMax, alpha, beta));
                    //Set alpha to best if best is bigger
                    alpha = max(alpha, best);

                    //Reset the cell to unoccupied
                    lB.grid[x][y] = 0;

                    //Prune
                    if (beta <= alpha)
                        goto maxPrune; //Exit loops
                }
            }
        }
        maxPrune:
        return best;
    }
    else //If it's the minimizer's move
    {
        int best = MAX; //1000

        // Traverse all cells
        for (int x = 0; x < bs; x++)
        {
            for (int y = 0; y < bs; y++)
            {
                // Check if cell is empty
                if (lB.grid[x][y] == 0)
                {
                    // Make the move
                    lB.grid[x][y] = opponent;

                    //Recursively call algorithm and choose min value
                    best = min(best, minimax(lB, depth - 1, !isMax, alpha, beta));
                    //Set beta to best if best is smaller
                    beta = min(beta, best);

                    //Reset the cell to unoccupied
                    lB.grid[x][y] = 0;

                    //Prune
                    if (beta <= alpha)
                        goto minPrune; //Exit loops
                }
            }
        }
        minPrune:
        return best;
    }
}*/
//endregion

//region Heuristic Evaluation
int MinimaxPlayer::evaluate(Board &lB)
{
    int spots = lB.freeCellsSize();

    //Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        //Check both players for a winner, first checking for a line, then using DFS
        if (lB.CheckLine(player) || evaluateDFS(player, lB))
            return (player);
        else if (lB.CheckLine(opponent) || evaluateDFS(opponent, lB))
            return (opponent);
    }
    return 0;
}

bool MinimaxPlayer::evaluateDFS(int playerType, Board &lB)
{
    stack<int> searchStack;
    vector<int> visitedStack;

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
        }
        else if (playerType == 1)
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

bool MinimaxPlayer::isMovesLeft(int **grid)
{
    for (int i = 0; i < bs; i++)
        for (int j = 0; j < bs; j++)
            if (grid[i][j] == 0)
                return true;
    return false;
}

#endif /* MINIMAX_H_ */