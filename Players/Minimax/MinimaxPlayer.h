//
// Created by ascle on 16/04/2021.
//

#ifndef MINIMAXPLAYER_H
#define MINIMAXPLAYER_H

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

    double Heuristic(Board board);
};

#endif //MINIMAXPLAYER_H
