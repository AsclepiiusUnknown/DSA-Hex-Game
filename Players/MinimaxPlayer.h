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
    double Minimax(Board board, double depth, bool isMax, double A, double B);

    bool DepthCutoff(double depth);

    Move ManualStarts(Board board);

    PathCell PathEvaluation(Board board);

    PathCell RecursiveEvaluation(Board board, PathCell src);

    stack <PathCell> ReconstructPath(PathCell src, PathCell final);
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

//    if (board->MoveNumber() < 3 && bs > 3)
//    {
//        Move manual = ManualStarts(*board);
//        if (manual.x >= 0 && manual.y >= 0)
//            return manual;
//        else
//            printf("\n ERROR: No ManualStart move found by Minimax.\n");
//    }

    priority_queue <Move> moves;
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

            if (tempBoard.CheckForWin(player)) //if the board is full
            {
                cout << setw(12) << "Winning Move Found!" << endl;
                return {r, c, 1};
            }
            printf("\nBEFORE\n");
            double value = Minimax(tempBoard, 0, false, MIN, MAX);
            
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

double MinimaxPlayer::Minimax(Board board, double depth, bool isMax, double A, double B)
{
    //cout << " Depth: " << depth << endl;
    vector <Cell> emptyCells = board.getFreeCells();
    if (emptyCells.empty())
    {
        printf("FATAL ERROR: All cells used but no win was detected by Minimax!");
        return board.Evaluation(0, 0);
    }
    else if (DepthCutoff(depth))
    {
        PathCell cut = PathEvaluation(board);
        //cout << cut.x << " " << cut.y << endl;
        if (cut.x != 0 && cut.y != 0)
        {
            printf("\nAFTER*****************\n");
            return static_cast<double>(cut.GetValue());
        }
        else
        {
            printf("\nAFTER*****************\n");
            return -bs;
        }
    }

    //random_shuffle(emptyCells.begin(), emptyCells.end());
    double bestVal = (isMax) ? MIN : MAX;
    int playerType = (isMax) ? player : opponent;

    for (Cell i: emptyCells)
    {
        Board tempBoard(board);
        if (!tempBoard.AddTestMove(playerType, i.x, i.y))
            printf("ERROR: Invalid input created by Minimax's Max Move function\n");

        int status = tempBoard.Evaluation(player, opponent);

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

        double thisVal = Minimax(tempBoard, depth + (0.1 * bs), !isMax, A, B);

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

    if (depth == 0)
        cout << "\n BEST: " << bestVal << " DEPTH: " << depth << endl;
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
    if (canCutoff && depth >= maxDepth && depth > (0.1 * bs))
    {
        return true;
    }
    else return false;
}

PathCell MinimaxPlayer::PathEvaluation(Board board)
{
//    printf("\n HIT 2\n");
//
//    int bs = board.getBoardSize();
//    int **grid = board.grid;
//    for (int i = 0; i < bs; i++)
//    {
//        cout << endl << "| ";
//        for (int j = 0; j < bs; j++)
//        {
//            cout << grid[i][j] << " | ";
//        }
//    }
//
//    cout << endl;

    //int endGoal = bs - 1;

    vector <PathCell> search;

    for (int i = 0; i < bs; i++)
    {
        if (player == -1 && board.grid[i][0] == -1) // O
            search.push_back(PathCell(i, 0, NULL));
        else if (player == 1 && board.grid[0][i] == 1)
            search.push_back(PathCell(0, i, NULL));
    }

    PathCell best(-1, -1, NULL, -100, 100, search);

    if (search.empty())
    {
        return best;
    }

    for (PathCell c : search)
    {
        PathCell path = ReconstructPath(c, RecursiveEvaluation(board, c)).top();
        if (path.GetValue() > best.GetValue())
            best = path;
    }

    return best;
}

PathCell MinimaxPlayer::RecursiveEvaluation(Board board, PathCell src)
{
//    printf("\nRecursive Start\n");
    int h = src.h;
    if ((player && h == src.x) || (!player && h == src.y))
        h++;
    int length = src.length + 1.0;
    vector <PathCell> visited = src.visited;
    visited.push_back(src);

    PathCell best(-1, -1, NULL, -100, 100, visited);

    stack <Cell> n = board.CheckNeighbours(player, src.x, src.y);
    if (n.empty())
    {
//        printf("\nRecursive is Empty\n");
        return src;
    }
    else
    {
        //best = PathCell(n.top().x, n.top().y, &src, h, length, visited);
        while (!n.empty())
        {
//            printf("\nRecursive N\n");

            PathCell nPath(n.top().x, n.top().y, &src, h, length, visited);
            if (!board.isInVector(src.visited, nPath))
            {
                PathCell c = RecursiveEvaluation(board, nPath);
                if (c.GetValue() > best.GetValue())
                    best = c;
            }
            n.pop();
        }
    }
//    printf("\nRecursive End\n");
    if (best.x == -1 && best.y == -1)
        return src;
    return best;
}

stack <PathCell> MinimaxPlayer::ReconstructPath(PathCell src, PathCell final)
{
//    printf("\nReconstruct Start\n");

    stack <PathCell> path;
    PathCell next = final;
    int failsafe = 100;
    while (failsafe > 0)
    {
//        printf("\nReconstructing...\n");

        path.push(next);
//        printf("\n1\n");

        if (src.Compare(path.top()))
        {
//            printf("\nReconstruction End With Success\n");
            return path;
        }
//        printf("\n2\n");

        if (next.parent == NULL)
        {
            printf("\nReconstruct Error:\n");
//            cout << next.x << " " << next.y << endl;
//            cout << src.x << " " << src.y << endl;
        }

        next = *next.parent;
        failsafe--;
        printf("\n3\n");
    }
    if (path.empty())
        printf("\nERROR: Reconstruction of Minimax Path resulted in a path with no elements.\n");
    printf("\nReconstruction End with Failure\n");
    return path;
}
//endregion
#endif /* MINIMAX_H_ */