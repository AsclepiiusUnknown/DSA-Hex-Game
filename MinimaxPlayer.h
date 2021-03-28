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
    MinimaxPlayer(int t, string name = "Minimax") : // <--- add board here? is that possible/good (pointers?)??
            Player(t, name)
    {
    }

    struct Move
    {
        int x, y; //coordinates for the move
    };

    const int MAX = 1000;
    const int MIN = -1000;
    const int WIN_VAL = 10;

    //region Functions
    bool getMove(Board *, int &, int &);

    bool isMovesLeft(int **grid, int boardSize);

    int evaluate(Board *board);

    bool evaluateDFS(int playerType, Board *board);

    int minimax(Board *board, int depth, bool isMax, int alpha, int beta);

    Move findBestMove(Board *board);

    bool isInVector(vector<int> v, int e);
    //endregion
};

bool MinimaxPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    //Start the algorithm to get the best move
    Move bestMove = findBestMove(board);

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

//region Minimax Algorithm
MinimaxPlayer::Move MinimaxPlayer::findBestMove(Board *board)
{
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType();
    int bestVal = (player == -1) ? MIN : MAX;
    bool isMax = (player == 1);
    Move bestMove;
    bestMove.x = -1;
    bestMove.y = -1;

    //Traverse all cells
    for (int i = 0; i < bs; i++)
    {
        for (int j = 0; j < bs; j++)
        {
            //Check if cell is empty
            if (grid[i][j] == 0)
            {
                //Start by making the cell ours
                grid[i][j] = player;

                //Evaluate the move's value using minimax algorithm & alpha-beta pruning
                int moveVal = minimax(board, bs, isMax, MIN, MAX);

                //Reset the cell to unoccupied
                grid[i][j] = 0;

                //If the move being tested is our current best
                if (moveVal > bestVal)
                {
                    //Set to best values
                    bestMove.x = i;
                    bestMove.y = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    //Clear console then print the value of the best move
    system("CLS");
    cout << "The value of the best Move is : " << bestVal << endl;

    return bestMove;
}

int MinimaxPlayer::minimax(Board *board, int depth, bool isMax, int alpha, int beta)
{
    int score = evaluate(board);
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType(), opponent = (getType() * -1);

    //If the board is full without a winner then there is a tie
    if (!isMovesLeft(grid, bs))
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
                if (grid[x][y] == 0)
                {
                    //Start by making the cell ours
                    grid[x][y] = player;

                    //Recursively call algorithm and choose max value
                    best = max(best, minimax(board, depth - 1, !isMax, alpha, beta));
                    //Set alpha to best if best is bigger
                    alpha = max(alpha, best);

                    //Reset the cell to unoccupied
                    grid[x][y] = 0;

                    //Prune
                    if (beta <= alpha)
                        goto maxPrune; //Exit loops
                }
            }
        }
        maxPrune:
        return best;
    } else //If it's the minimizer's move
    {
        int best = MAX; //1000

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

                    //Recursively call algorithm and choose min value
                    best = min(best, minimax(board, depth - 1, !isMax, alpha, beta));
                    //Set beta to best if best is smaller
                    beta = min(beta, best);

                    //Reset the cell to unoccupied
                    grid[x][y] = 0;

                    //Prune
                    if (beta <= alpha)
                        goto minPrune; //Exit loops
                }
            }
        }
        minPrune:
        return best;
    }
}
//endregion

//region Heuristic Evaluation
int MinimaxPlayer::evaluate(Board *board)
{
    int spots = board->FSpotsSize();
    int bs = board->getBoardSize();

    //Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        int player = getType(), opponent = (getType() * -1);

        //Check both players for a winner, first checking for a line, then using DFS
        if (board->lineWin(player) || evaluateDFS(player, board))
            return player * WIN_VAL;
        else if (board->lineWin(opponent) || evaluateDFS(opponent, board))
            return opponent * WIN_VAL;
    }
    return 0; //No winner
}

bool MinimaxPlayer::evaluateDFS(int playerType, Board *board)
{
    int bs = board->getBoardSize();
    stack<int> searchStack;
    vector<int> visitedStack;

    //Add all freeSpots to be searched for a win
    for (int i = 0; i < board->ASpotsSize(); i++)
    {
        searchStack.push(board->getASpots()[i]);
    }

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
        } else
        {
            stack<int> children = board->checkNeighbours(playerType, sX, sY);
            while (!children.empty())
            {
                if (!isInVector(visitedStack, children.top()))
                    searchStack.push(children.top());

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
