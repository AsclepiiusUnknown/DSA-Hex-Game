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
    const int WIN_VAL = 50;
    const int MAX_DEPTH = 100;

    int bs;

    int player = getType();
    int opponent = player * -1;

    //region Functions
    bool getMove(Board *, int &, int &);

    bool isMovesLeft(int **grid);

    int evaluate(Board &lB, int depth);

    bool evaluateDFS(int playerType, Board &lB);

    int minimax(Board &lB, int depth, bool isMax, int alpha, int beta);

    MinimaxPlayer::Move findBestMove(Board lB);

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

    //Create a board
    Board lB = *board;
    //Setup the global bs (board size) variable
    bs = lB.getBoardSize();

    //Start the algorithm recursion to get the best move
    Move bestMove = findBestMove(lB);

    //Use the coordinates of our 'best move'
    x = bestMove.x;
    y = bestMove.y;

    //Check if our coordinate values are valid and log an error if not
    if (!board->validInput(x, y))
    {
        cout << "ERROR: Minimax input was invalid" << endl;
        return false;
    }

    return true;
}

//region Minimax Algorithm
MinimaxPlayer::Move MinimaxPlayer::findBestMove(Board lB)
{
    int bestVal = (player == -1) ? MIN : MAX; //Set opposite values (P-1 wants to get a big value so it's preset is MIN)
    bool isMax = (player == 1); //AI isn't the maximiser
    Move bestMove;
    bestMove.x = -1;
    bestMove.y = -1;

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

                //Evaluate the move's value using minimax algorithm & alpha-beta pruning
                int moveVal = minimax(lB, MAX_DEPTH, isMax, MIN, MAX);

                //Reset the cell to unoccupied
                lB.grid[x][y] = 0;

                //If the move being tested is our current best
                if (moveVal > bestVal)
                {
                    //Set to best values
                    bestMove.x = x;
                    bestMove.y = y;
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

int MinimaxPlayer::minimax(Board &lB, int depth, bool isMax, int alpha, int beta)
{
    int score = evaluate(lB, depth);

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
    } else //If it's the minimizer's move
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
}
//endregion

//region Heuristic Evaluation
int MinimaxPlayer::evaluate(Board &lB, int depth)
{
    int spots = lB.FSpotsSize();
    int eval = MAX_DEPTH - depth;

    //Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        //Check both players for a winner, first checking for a line, then using DFS
        if (lB.lineWin(player) || evaluateDFS(player, lB))
            eval += (player * WIN_VAL);
        else if (lB.lineWin(opponent) || evaluateDFS(opponent, lB))
            eval += (opponent * WIN_VAL);
    }
    return eval;
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

bool MinimaxPlayer::isMovesLeft(int **grid)
{
    for (int i = 0; i < bs; i++)
        for (int j = 0; j < bs; j++)
            if (grid[i][j] == 0)
                return true;
    return false;
}

#endif /* MINIMAX_H_ */
