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
    MinimaxPlayer(int t, string name = "Minimax") : // <--- add board here? is that possible??
            Player(t, name)
    {
    }

    struct Move
    {
        int x, y; //row and column for each move
    };

    //region Functions
    bool getMove(Board *, int &, int &);

    bool isMovesLeft(int **grid, int boardSize);

    int evaluate(Board *board);

    bool evaluateDFS(int playerType, Board *board);

    int minimax(Board *board, int depth, bool isMax);

    Move findBestMove(Board *board);

    //endregion
    bool isInVector(vector<int> v, int e);
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

    if (!board->validInput(x, y))
    {
        cout << "ERROR: Minimax input was invalid" << endl;
        return false;
    }

    return true;
}

//region Minimax Algorithm
MinimaxPlayer::Move MinimaxPlayer::findBestMove(Board *board)
{
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int bestVal = -1000;
    int player = getType();
    bool isMax = (player == 1);
    Move bestMove;
    bestMove.x = -1;
    bestMove.y = -1;

    // Traverse all cells, evaluate minimax function for
    // all empty cells. And return the cell with optimal
    // value.
    for (int i = 0; i < bs; i++)
    {
        for (int j = 0; j < bs; j++)
        {
            // Check if cell is empty
            if (grid[i][j] == 0)
            {
                // Make the move
                grid[i][j] = player;

                // compute evaluation function for this
                // move.
                int moveVal = minimax(board, 0, isMax); // <-- error on first iteration

                // Undo the move
                grid[i][j] = 0;

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

int MinimaxPlayer::minimax(Board *board, int depth, bool isMax)
{
    int score = evaluate(board);
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType(), opponent = (getType() * -1);

    // If the board is full without a winner then there is a tie
    if (!isMovesLeft(grid, bs))
        return 0;

    // If Maximizer has won, return its score
    if (score == 10)
        return score;

    // If Minimizer has won, return its score
    if (score == -10)
        return score;

    // If it's the Maximizer's move
    if (isMax)
    {
        int best = -1000;

        // Traverse all cells
        for (int x = 0; x < bs; x++)
        {
            for (int y = 0; y < bs; y++)
            {
                // Check if cell is empty
                if (grid[x][y] == 0)
                {
                    // Make the move
                    grid[x][y] = player;

                    // Call minimax recursively and choose
                    // the maximum value
                    best = max(best, minimax(board, depth + 1, !isMax));

                    // Undo the move
                    grid[x][y] = 0;
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
        for (int x = 0; x < bs; x++)
        {
            for (int y = 0; y < bs; y++)
            {
                // Check if cell is empty
                if (grid[x][y] == 0)
                {
                    // Make the move
                    grid[x][y] = opponent;

                    // Call minimax recursively and choose
                    // the minimum value
                    best = min(best, minimax(board, depth + 1, !isMax));

                    // Undo the move
                    grid[x][y] = 0;
                }
            }
        }
        return best;
    }
}
//endregion

//region Heuristic Evaluation
int MinimaxPlayer::evaluate(Board *board)
{
    int spots = board->getSpotsSize();
    int bs = board->getBoardSize();

    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        int player = getType(), opponent = (getType() * -1);

        if (board->lineWin(player) || evaluateDFS(player, board))
            return player * 10;
        else if (board->lineWin(opponent) || evaluateDFS(opponent, board))
            return opponent * 10;
    }
    return 0;
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
                if (!isInVector(visitedStack, children.top()))
                    trackStack.push(children.top());

                children.pop();
            }
        }
    }

    return false;
}

bool MinimaxPlayer::isInVector(vector<int> v, int e)
{
    if (v.empty())
        return false;

    for (int i : v)
        if (i == e)
            return true;

    return false;
}
//endregion

bool MinimaxPlayer::isMovesLeft(int **grid, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (grid[i][j] == 0)
                return true;
    return false;
}

#endif /* MINIMAX_H_ */
