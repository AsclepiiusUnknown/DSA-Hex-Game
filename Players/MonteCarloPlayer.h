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

using namespace std;

class MonteCarloPlayer : public Player
{
public:
    MonteCarloPlayer(int t, string name = "Monte Carlo") :
            Player(t, name)
    {
    }

    struct Move
    {
        int x, y; //coordinates for the move
        double v; //Move value

        Move(int _x, int _y, double _v) : x(_x), y(_y), v(_v)
        {}

        bool operator<(const Move &m) const
        {
            return v < m.v;
        };
    };

    const int WIN_VAL = 10;

    int player = type, opponent = (type * -1);

    bool getMove(Board *, int &, int &);

    double simulation(Board board);

    double expansion(int playerType, Board board, double depth);

    int evaluate(Board lB);

    bool evaluateDFS(int playerType, Board lB);

    void getRandomMove(int &x, int &y, Board board);
};

bool MonteCarloPlayer::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: Random player can't move cause the board is full!" << endl;
        return false;
    }

    int bs = board->getBoardSize();

    priority_queue<Move> moves;
    cout << "Values of Moves: " << endl;
    for (int r = 0; r < bs; r++)
    {
        for (int c = 0; c < bs; c++)
        {
            board->printCoord(r, c, false);

            if (board->grid[r][c] != 0)
            {
                cout << setw(5) << "X" << endl;
                continue;
            }

            Board tempBoard(*board);
            tempBoard.grid[r][c] = player;
            tempBoard.removeFreeCell(r, c);
            //tempBoard.addMove(player, r, c);

            //fixme cout << "board " << board->freeCellsSize() << endl;
            //fixme cout << "temp " << tempBoard.freeCellsSize() << endl;

            if (tempBoard.checkWinningStatus(player, r, c))
            {
                x = r;
                y = c;
                return true;
            }

            double utility = simulation(tempBoard);

            Move m(r, c, utility);
            moves.push(m);
            cout << setw(5) << m.v << endl;
        }
        cout << endl;
    }

    if (!moves.empty())
    {
        x = moves.top().x;
        y = moves.top().y;
        cout << "Best value: " << moves.top().v << endl;
        return true;
    }

    return false;
}

double MonteCarloPlayer::simulation(Board board)
{
    int times = 10000;
    double winning = 0.0;

    for (int i = 0; i < times; i++)
    {
        Board tempBoard(board);
        winning += expansion(opponent, tempBoard, 0);
    }
    return (winning / (double) times);
}

double MonteCarloPlayer::expansion(int playerType, Board board, double depth)
{
    int status = evaluate(board);
    //fixme cout << status << endl;

    //If this player is winning
    if (status == player)
        return 1.0 - depth;
    else if (status == opponent)
        return -1.0 + depth;
    else if (status != 0) //Continue if we aren't finished and no winner
        return 0.0;

    int x, y;
    getRandomMove(x, y, board);
    board.grid[x][y] = player;
    board.removeFreeCell(x, y);
    playerType *= -1;
    depth += 0.05;
    return expansion(playerType, board, depth);
}

void
MonteCarloPlayer::getRandomMove(int &x, int &y, Board board)//fixme used to have spots passed through here but caused issues because spots wasnt being updated when using grid to add manually
{
    vector<int> spots = board.getFreeCells();
    int bs = board.getBoardSize();

    int i = rand() % spots.size();
    x = i / bs;
    y = i % bs;
}

//region Heuristic Evaluation
int MonteCarloPlayer::evaluate(Board lB)
{
    int spots = lB.freeCellsSize();
    int bs = lB.getBoardSize();
    //fixme cout << spots << endl;

    // Only check for a win if enough cells have been occupied
    if ((spots + (bs * 2 - 1)) <= (bs * bs))
    {
        //Check both players for a winner, first checking for a line, then using DFS
        if (lB.lineWin(player) || evaluateDFS(player, lB))
            return (player);
        else if (lB.lineWin(opponent) || evaluateDFS(opponent, lB))
            return (opponent);
    }

    if (spots > 0)
        return 0; //continue value

    return 5; //Error Check (should either be a win or some spots left)
}

bool MonteCarloPlayer::evaluateDFS(int playerType, Board lB)
{
    stack<int> searchStack;
    vector<int> visitedStack;
    int bs = lB.getBoardSize();

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

#endif /* MONTECARLOPLAYER_H_ */
