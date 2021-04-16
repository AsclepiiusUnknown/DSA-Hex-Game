#ifndef MINIMAX_H_
#define MINIMAX_H_

class MinimaxPlayer : public Player
{
public:
    MinimaxPlayer(int t, string symbol = "Undefined (ERROR)", double depth = 25, string name = "Minimax") : Player(t, symbol, name), maxDepth(depth)
    {}

    const int MAX = 10000;
    const int MIN = -10000;
    int WIN_VAL = 10;

    int bs;
    double maxDepth = 25;
    bool canCutoff = true;

    int player = type, opponent = type * -1;

    bool GetMove(Board *, int &, int &);

    Move BestMove(Board *board);

    double Minimax(Board board, double depth, bool isMax, double A, double B);

    bool DepthCutoff(double depth)
    {
        if (canCutoff && depth >= maxDepth && depth > (0.1 * bs))
        {
            return true;
        }
        else return false;
    }

    //region Evaluation
    PathCell PathEvaluation(Board board);

    PathCell MakePath(Board board, PathCell src);

    stack <PathCell> ReconstructPath(PathCell src, PathCell final);
    //endregion
};

bool MinimaxPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    //Setup the global bs (board size) variable
    bs = board->GetBoardSize();
    WIN_VAL = bs + 2;

    cout << " Max Depth: " << maxDepth << endl;
    cout << " Depth Increment: " << 0.01 * bs << endl;

    Move m = BestMove(board);
    x = m.x;
    y = m.y;

    if (!board->IsValidInput(x, y))
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

    priority_queue <Move> moves;
    cout << " Can Cutoff: " << canCutoff << endl;
    cout << "Values of Moves: " << endl;
    for (int r = 0; r < bs; r++)
    {
        for (int c = 0; c < bs; c++)
        {
            board->printCoord(r + 1, c + 1, false);

            if (board->GridValue(r, c) == 1)
            {
                cout << setw(8) << "X" << endl;
                continue;
            }
            else if (board->GridValue(r, c) == -1)
            {
                cout << setw(12) << "O" << endl;
                continue;
            }

            Board tempBoard(*board);
            tempBoard.AddTestMove(player, r, c);

            if (tempBoard.CheckForWin(player))
            {
                cout << setw(12) << "Winning Move Found!" << endl;
                return {r, c, 1};
            }
//            printf("\nBEFORE\n"); //todo*****************************************
            double value = Minimax(tempBoard, 0, false, MIN, MAX);
//            printf("\nAFTER""""""\n"); //todo*****************************************
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
    vector <Cell> emptyCells = board.GetEmpty();
    if (emptyCells.empty())
    {
        printf("FATAL ERROR: All cells used but no win was detected by Minimax!");
        return board.Evaluation(0, 0);
    }
    else if (DepthCutoff(depth))
    {
        PathCell cut = PathEvaluation(board);
        if (cut.x != 0 && cut.y != 0)
            return static_cast<double>(cut.GetValue());
        else
            return -bs;
    }

    //random_shuffle(emptyCells.begin(), emptyCells.end()); //todo********** add when functional!
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
            cout << "ERROR: Invalid evaluation status found in Minimax. Free Cells: " << tempBoard.EmptySize() << " Can Win: " << tempBoard.CanWin() << " Status: " << status << endl;
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
            break;
    }
    if ((bestVal == MAX || bestVal == MIN))
        printf("\nERROR: BestUtil was never changed so no value was given to the move.");

    if (depth == 0)
        cout << "\n BEST: " << bestVal << " DEPTH: " << depth << endl;
    return bestVal;
}

//SECTION - Evaluation
PathCell MinimaxPlayer::PathEvaluation(Board board)
{
    vector <PathCell> search;

    for (int i = 0; i < bs; i++)
    {
        if (player == -1 && board.GridValue(i, 0) == player)
            search.push_back(PathCell(i, 0, nullptr));
        else if (player == 1 && board.GridValue(0, i) == player)
            search.push_back(PathCell(0, i, nullptr));
    }

    PathCell best(-1, -1, nullptr, -100, 100, search);

    if (search.empty())
        return best;

    for (PathCell c : search)
    {
        PathCell path = ReconstructPath(c, MakePath(board, c)).top();
        printf("\nAFTER\n");

        if (path.GetValue() > best.GetValue())
            best = path;
    }

    return best;
}

PathCell MinimaxPlayer::MakePath(Board board, PathCell src)
{
    int h = src.h;
    if ((player && h == src.x) || (!player && h == src.y))
        h++;
    int length = src.length + 1.0;
    vector <PathCell> visited = src.visited;
    visited.push_back(src);

    PathCell best(-1, -1, nullptr, -100, 100, visited);

    stack <Cell> n = board.CheckNeighbours(player, src.x, src.y);
    if (n.empty())
    {
        return src;
    }
    else
    {
        while (!n.empty())
        {
            PathCell nPath(n.top().x, n.top().y, &src, h, length, visited);
            if (!board.isInVector(src.visited, nPath))
            {
                PathCell c = MakePath(board, nPath);
                if (c.GetValue() > best.GetValue())
                    best = c;
            }
            n.pop();
        }
    }
    if (best.x == -1 && best.y == -1)
    {
        printf("\nReturning Src\n");
        return src;
    }
    return best;
}

stack <PathCell> MinimaxPlayer::ReconstructPath(PathCell src, PathCell final)
{
    stack <PathCell> path;
    PathCell *next = &final;
    int failsafe = 100;
    while (failsafe > 0)
    {
        path.push(*next);
        printf("\nRECONSTRUCT\n");

        if (path.top().parent == nullptr)
            return path;

        if (next->parent == nullptr)
            printf("\nReconstruct Error:\n");

        next = next->parent;
        failsafe--;
    }
    if (path.empty())
        printf("\nERROR: Reconstruction of Minimax Path resulted in a path with no elements.\n");
    printf("\nReconstruction End with Failure\n");
    return path;
}
//endregion
#endif /* MINIMAX_H_ */