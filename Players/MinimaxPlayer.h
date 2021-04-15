#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "../AStar.h"

class MinimaxPlayer : public Player
{
public:
    MinimaxPlayer(int t, string symbol = "Undefined (ERROR)", double depth = 25, string name = "Minimax") :
            Player(t, symbol, name), maxDepth(depth)
    {
    }

    const int MAX = 10000;
    const int MIN = -10000;
    int WIN_VAL = 5; //250/100 (2.5) because 225 is the largest possible depth that can be simulated since we cap bs at 15 (15^2)

    int bs;
    double maxDepth = 25;
    bool canCutoff = true;

    int player = type, opponent = type * -1;

    AStar aStar;

    //region Functions
    bool GetMove(Board *, int &, int &);

    Move BestMove(Board *board);

    //endregion
    double Minimax(Board board, double depth, bool isMax, double A, double B, Cell src);

    bool DepthCutoff(double depth);

    Move ManualStarts(Board board);

    Move ScopeAnalysis(int scope, Board board);

    double AnalyzePos(int x, int y, vector<Cell> oCells, vector<Cell> pCells, int scope);
};

bool MinimaxPlayer::GetMove(Board *board, int &x, int &y)
{
    cout << " Max Depth is " << maxDepth << endl;

    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    //Setup the global bs (board size) variable
    bs = board->getBoardSize();
    WIN_VAL = bs + 2;
    maxDepth = bs * maxDepth;

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
    canCutoff = (board->MoveNumber() < (((bs - 1) * 2) - 1));

    if (board->MoveNumber() < 3 && bs > 3)
    {
        Move manual = ManualStarts(*board);
        if (manual.x >= 0 && manual.y >= 0)
            return manual;
        else
            printf("\n ERROR: No ManualStart move found by Minimax.\n");
    }

    priority_queue<Move> moves;
    cout << " Can Cutoff: " << canCutoff << endl;
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
                cout << setw(12) << "O" << endl;
                continue;
            }

            Board tempBoard(*board);
            tempBoard.AddTestMove(player, r, c);

            if (tempBoard.CheckForWin(player, r, c)) //if the board is full
            {
                cout << setw(12) << "Winning Move Found!" << endl;
                return {r, c, 1};
            }
            double value = Minimax(tempBoard, 0, false, MIN, MAX, Cell(0, 0));

            Move m(r, c, value);
            moves.push(m);
            cout << setw(12) << value << endl;
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

double MinimaxPlayer::Minimax(Board board, double depth, bool isMax, double A, double B, Cell src)
{
    //cout << "\n Depth is " << depth << endl;
    vector<Cell> emptyCells = board.getFreeCells();
    if (emptyCells.empty() || (canCutoff && DepthCutoff(depth)))
    {
        if (emptyCells.empty())
            printf("FATAL ERROR: All cells used but no win was detected by Minimax!");
        if (!canCutoff)
            printf("\n**Cutoff Error**\n");
        printf("MARK1");
        stack<ASCell> bestPath = aStar.EvaluateAStar(board, player, src);
        if (bestPath.empty())
            printf("EMPTY");
        return (WIN_VAL - bestPath.top().F()) - depth;
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
            return -WIN_VAL + depth;
        }
        else if (status != 0)
        {
            cout << "ERROR: Invalid evaluation status found in Minimax. Free Cells: " << tempBoard.freeCellsSize() << " Can Win: " << tempBoard.CanWin() << " Status: " << status << endl;
            return 0.0;
        }

        double thisVal = Minimax(tempBoard, depth + (0.1 * bs), !isMax, A, B, src);

        if (isMax)
        {
            if (thisVal > bestVal)
                bestVal = thisVal;
            if (A < bestVal)
                A = bestVal;
        }
        else
        {
            if (thisVal < bestVal)
                bestVal = thisVal;
            if (B > bestVal)
                B = bestVal;
        }

        if (A >= B)
        {
            //printf(" **PRUNED** ");
            break;
        }
        //cout << endl << " A: " << A << " B: " << B << " this: " << thisVal << " best: " << bestVal << endl;
    }
    if ((bestVal == MAX || bestVal == MIN))
        printf("\nERROR: BestUtil was never changed so no value was given to the move.");

    return bestVal;
}

Move MinimaxPlayer::ManualStarts(Board board)
{
    int move = board.MoveNumber();
    if (move == 1)
    {
        if (player == 1)
            return {bs - 1, 0, 2.5};//bottom left
        else
            return {0, bs - 1, 2.5};//top right
    }
    else if (move == 2)
    {
        int **grid = board.getGrid();
        if (grid[bs - 1][0] == opponent) //if they played the bottom left
        {
            if (grid[bs - 2][0] == 0)
                return {bs - 2, 0, 2.5};
            else if (grid[bs - 2][1] == 0)
                return {bs - 2, 1, 2.5};

        }
        else if (grid[0][bs - 1] == opponent) //if they played the top right
        {
            if (grid[1][bs - 1] == 0)
                return {1, bs - 1, 2.5};
            else if (grid[1][bs - 2] == 0)
                return {1, bs - 2, 2.5};
        }
        else
        {
            if (grid[bs - 1][0] == 0)
                return {bs - 1, 0, 2.5};
            else if (grid[0][bs - 1] == 0)
                return {0, bs - 1, 2.5};
        }
    }

    return {-1, -1, 0};
}

bool MinimaxPlayer::DepthCutoff(double depth)
{
    if (depth >= maxDepth && depth > 0.1)
    {
        return true;
    }
    else return false;
}
//endregion
#endif /* MINIMAX_H_ */