/*
 * RandomPlayer.h
 *
 *  Created on: 22/03/2021
 *      Author: Broderick Westrope
 */

#ifndef MINIMAX_H_
#define MINIMAX_H_

#include "queue"

using namespace std;

class MiniMaxPlayerBad : public Player
{
public:
    MiniMaxPlayerBad(int t, string name = "Bad Mini Max") :
            Player(t, name)
    {
    }

    //region Move Class
    struct Move
    {
        int score;
        int pos; //the index x*n+y
        int x, y;

        Move(int score, int pos, int bs) : score(score), pos(pos), x(pos / bs), y(pos % bs)
        {}

        Move(int score, int x, int y, int bs) : score(score), pos(x * bs + y), x(x), y(y)
        {}
    };

    struct CompareHeight
    {
        bool operator()(Move const &m1, Move const &m2)
        {
            return m1.score > m2.score;
        }
    };
    //endregion

    //region Functions
    bool getMove(Board *, int &, int &);

    int minimax(Board *board, int **grid, int player, int boardSize);

    void printPQ(const priority_queue<Move, vector<Move>, CompareHeight> &moves, int boardSize);
    //endregion
};

bool MiniMaxPlayerBad::getMove(Board *board, int &x, int &y)
{
    if (board->isBoardFull())
    {
        cout << "ERROR: MiniMax player can't move cause the board is full!" << endl;
        return false;
    }

    int **grid = board->getGrid();
    int boardSize = board->getBoardSize();
    priority_queue<Move, vector<Move>, CompareHeight> moves;
    //int move = -1;
    //int score = -2;

    for (int r = 0; r < boardSize; ++r)
        for (int c = 0; c < boardSize; c++)
        {
            if (grid[r][c] == 0)
            {
                grid[r][c] = getType();
                int tempScore = -minimax(board, grid, getType(), boardSize);
                grid[r][c] = 0;

                moves.push(Move(tempScore, r, c, boardSize));

//                if (tempScore > score)
//                {
//                    score = tempScore;
//                    move = r * boardSize + c;
//                    moves.push(Move());
//                }
            }
        }

    x = moves.top().x;
    y = moves.top().y;

    printPQ(moves, boardSize);

    return true;
}

int MiniMaxPlayerBad::minimax(Board *board, int **grid, int player, int boardSize)
{
    //int move = -1;
    //int score = -2;

    priority_queue<Move, vector<Move>, CompareHeight> moves;

    for (int r = 0; r < boardSize; ++r)
        for (int c = 0; c < boardSize; c++)
        {
            if (grid[r][c] == 0)
            {
                grid[r][c] = player;
                int thisScore = -minimax(board, grid, player * -1, boardSize);
                grid[r][c] = 0;//Reset board after try

                moves.push(Move(thisScore, r, c, boardSize));

                if (board->checkWinningStatus(player, moves.top().x, moves.top().y))
                    return player;

//                if (thisScore > score)
//                {
//                    score = thisScore;
//                    move = r * boardSize + c;
//                    moves.push(move);
//                }
            }
        }
//    if (move == -1)
//        return 0;

    return moves.top().score;
}

void MiniMaxPlayerBad::printPQ(const priority_queue<Move, vector<Move>, CompareHeight> &moves, int boardSize)
{
    priority_queue<Move, vector<Move>, CompareHeight> g = moves;
    while (!g.empty())
    {
        int x = g.top().x;
        int y = g.top().y;
        cout << x << "," << y << " ";
        g.pop();
    }
    cout << '\n';
}

#endif /* MINIMAX_H_ */
