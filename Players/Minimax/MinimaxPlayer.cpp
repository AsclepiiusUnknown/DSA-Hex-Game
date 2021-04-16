#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "MinimaxPlayer.h"

bool MinimaxPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    bs = board->GetBoardSize();
    WIN_VAL = bs + 2;
    
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
    vector <Cell> emptyCells = board.GetEmpty();
    if (emptyCells.empty())
    {
        printf("FATAL ERROR: All cells used but no win was detected by Minimax!");
        return board.Evaluation(player, opponent) - depth;
    }
    else if (depth >= maxDepth)
    {
        return Heuristic(board) - depth;
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

        double thisVal = Minimax(tempBoard, depth + 0.1, !isMax, A, B);

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

double MinimaxPlayer::Heuristic(Board board)
{
    bool thisScore[bs];
    int bestScore = 0;
    int thisLength = 0;

    for (int i = 0; i < bs; i++)
    {
        for (int j = 0; j < bs; j++)
            thisScore[j] = false;

        stack <Cell> path;
        vector <Cell> visited;

        if (player == 1 && board.GetGrid()[0][i] == player)
        {
            path.push(Cell(0, i));
            thisScore[0] = true;
        }
        else if (player == -1 && board.GetGrid()[i][0] == player)
        {
            path.push(Cell(i, 0));
            thisScore[0] = true;
        }
        else
            continue;

        while (!path.empty())
        {
            thisLength++;
            Cell current = path.top();
            path.pop();
            visited.push_back(current);

            stack <Cell> n = board.CheckNeighbours(player, current.x, current.y);
            while (!n.empty())
            {
                if (n.top().x == -1 || n.top().y == -1)//todo whats this for?????
                    return type;

                if (board.isInVector(visited, n.top()))
                {
                    path.push(n.top());
                }

                if (player == 1)
                {
                    thisScore[n.top().x] = true;
                }
                else
                {
                    thisScore[n.top().y] = true;
                }

                n.pop();
            }
        }
        int k = 0;
        for (; k < bs; k++)
            if (!thisScore[k])
                break;

        double value = (((double) k * 2.0) - (double) thisLength);

        if (value > bestScore)
            bestScore = k;
    }

    return
            bestScore;
}

#endif /* MINIMAX_H_ */