#ifndef MONTECARLOPLAYER_H_
#define MONTECARLOPLAYER_H_

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include "vector"
#include "queue"
#include "time.h"
#include "../Cell.h"

using namespace std;

class MonteCarloPlayer : public Player
{
public:
    MonteCarloPlayer(int t, string symbol = "Undefined (ERROR)", double times = 2500, string name = "Monte Carlo") :
            Player(t, symbol, name), times(times)
    {
    }

    int player = type, opponent = -type, bs;
    double times;

    bool GetMove(Board *board, int &x, int &y);

    double Simulation(Board board);

    double Expansion(int playerType, Board board, double depth);

    Cell RandomMove(Board board);

    Move BestMove(Board *board);
};

bool MonteCarloPlayer::GetMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    bs = board->getBoardSize();
    Move m = BestMove(board);
    x = m.x;
    y = m.y;

    //Check if our coordinate values are valid and log an error if not
    if (!board->validInput(x, y))
    {
        cout << "ERROR: Monte Carlo input was invalid" << endl;
        return false;
    }

    return true;
}

Move MonteCarloPlayer::BestMove(Board *board)
{
    priority_queue<Move> moves;
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
                cout << setw(8) << "O" << endl;
                continue;
            }

            Board tempBoard(*board);
            tempBoard.AddTestMove(player, r, c);

            if (tempBoard.CheckForWin(player, r, c))
            {
                printf("Winning move found!");
                return {r, c, 1};
            }

            double value = Simulation(tempBoard);

            Move m(r, c, value);
            moves.push(m);
            cout << setw(8) << value << endl;
        }
        cout << endl;
    }

    if (!moves.empty())
    {
        cout << "Best value: " << moves.top().v << endl;
        return moves.top();
    }

    cout << "ERROR: No appropriate move was found by Monte Carlo" << endl;
    return {-1, -1, 0};
}

double MonteCarloPlayer::Simulation(Board board)
{
    double winning = 0.0;

    for (int i = 0; i < times; i++)
    {
        Board tempBoard(board);
        winning += Expansion(opponent, tempBoard, 0);
    }
    return (winning / (double) times);
}

double MonteCarloPlayer::Expansion(int playerType, Board board, double depth)
{
    Cell c = RandomMove(board);
    if (!board.AddTestMove(playerType, c.x, c.y))
        printf("ERROR: Invalid input created by Monte-Carlo's Random Move function\n");

    int status = board.Evaluation(c.x, c.y, player, opponent);

    if (status == player)
    {
        return 1.0 - depth;
    }
    else if (status == opponent)
    {
        return -1.0 - depth;
    }
    else if (status != 0)
    {
        cout << "ERROR: Invalid evaluation status found in Monte Carlo" << endl;
        return 0.0;
    }

    playerType = (playerType == player) ? opponent : player;
    return Expansion(playerType, board, depth + 0.05);
}

Cell MonteCarloPlayer::RandomMove(Board board)
{
    vector<Cell> spots = board.getFreeCells();

    int i = rand() % spots.size();
    int x = spots[i].x;
    int y = spots[i].y;

    return Cell(x, y);
}

#endif /* MONTECARLOPLAYER_H_ */
