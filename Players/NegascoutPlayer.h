//region Definitions
/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef NEGASCOUT_H_
#define NEGASCOUT_H_
//endregion

class NegascoutPlayer : public Player
{
public:
    NegascoutPlayer(int t, string symbol = "Undefined (ERROR)", string name = "Negascout") :
            Player(t, symbol, name)
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
    bool GetMove(Board *, int &, int &);

    bool isMovesLeft(int **grid, int boardSize);

    int evaluate(Board *board);

    bool evaluateDFS(Board *board, int playerType);

    int minimax(Board *board, int depth, bool isMax, int alpha, int beta);

    Move findBestMove(Board *board);

    bool isInVector(vector<Cell> v, Cell e);

    //endregion
};

bool NegascoutPlayer::GetMove(Board *board, int &x, int &y)
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
NegascoutPlayer::Move NegascoutPlayer::findBestMove(Board *board)
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
                int moveVal = minimax(board, isMax, false, 0, 0);

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
    cout << "The value of the best Cell is : " << bestVal << endl;

    return bestMove;
}

int NegascoutPlayer::minimax(Board *board, int depth, bool isMax, int alpha, int beta)
{
    int bestScore = evaluate(board);
    int **grid = board->getGrid();
    int bs = board->getBoardSize();
    int player = getType(), opponent = (getType() * -1);


    if (depth == 0)
        return bestScore;

    bestScore = MIN; //-1000

    //Recursively call algorithm and choose max value
    bestScore = -minimax(board, depth + 1, !isMax, -beta, -alpha);
    if (bestScore > alpha)
    {
        if (bestScore >= beta)
            return bestScore; //this is a refutation move, not a pv move. however it will usually cutoff so it can be considered a best move if no other is found

        alpha = bestScore;
    }

    ///

    // Traverse all cells
    for (int x = 0; x < bs; x++)
    {
        for (int y = 0; y < bs; y++)
        {
            // Check if cell is empty
            if (grid[x][y] == 0)
            {
                int score;
                //score = zWSearch(-alpha, board);
                if (score > alpha && score < beta)
                {
                    bestScore = -minimax(board, depth - 1, isMax, -beta, -alpha);
                    if (score > alpha)
                    {
                        alpha = score;
                    }
                }

                if (score > bestScore)
                    if (score >= beta)
                        return score;
                bestScore = score;


            }
        }
    }
    return bestScore;
}
//endregion

//region Heuristic Evaluation
int NegascoutPlayer::evaluate(Board *board)
{
    int spots = board->freeCellsSize();
    int bs = board->getBoardSize();

    //Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        int player = getType(), opponent = (getType() * -1);

        //Check both players for a winner, first checking for a line, then using DFS
        if (board->CheckLine(player) || evaluateDFS(board, player))
            return player * WIN_VAL;
        else if (board->CheckLine(opponent) || evaluateDFS(board, opponent))
            return opponent * WIN_VAL;
    }
    return 0; //No winner
}

bool NegascoutPlayer::evaluateDFS(Board *board, int playerType) //Iterative Deepening Depth-First Search
{
    int bs = board->getBoardSize();
    stack<Cell> searchStack;
    vector<Cell> visitedStack;

    //Add all emptyCells to be searched for a win
    for (int i = 0; i < board->allCellsSize(); i++)
    {
        searchStack.push(board->getAllCells()[i]);
    }

    if (searchStack.empty())
        return false;

    bool start = false, finish = false;
    int startGoal = 0, endGoal = bs - 1;

    while (!searchStack.empty())
    {
        Cell s = searchStack.top();
        searchStack.pop();
        visitedStack.push_back(s);

        if (playerType == -1)
        {
            if (s.y == startGoal)
                start = true;
            else if (s.y == endGoal)
                finish = true;
        }
        else if (playerType == 1)
        {
            if (s.x == startGoal)
                start = true;
            else if (s.x == endGoal)
                finish = true;
        }

        if (start && finish)
        {
            return true;
        }
        else
        {
            stack<Cell> children = board->CheckNeighbours(playerType, s.x, s.y);
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

bool NegascoutPlayer::isInVector(vector<Cell> v, Cell e)
{
    if (v.empty())
        return false;

    for (Cell i : v)
        if (i.x == e.x && i.y == e.y)
            return true;

    return false;
}
//endregion

bool NegascoutPlayer::isMovesLeft(int **grid, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (grid[i][j] == 0)
                return true;
    return false;
}

#endif /* MINIMAX_H_ */
