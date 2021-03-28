/*
 * Main.cpp
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#include<iostream>
#include<string>

using namespace std;

#include "Board.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "HexGame.h"
#include "RandomPlayer.h"
#include "MonteCarloPlayer.h"
#include "MinimaxPlayer.h"
#include "NegascoutPlayer.h"

int main()
{
    //region Player Pre-Game Input
    int p2Type = 1;
    cout << "Who do you want to verse? (0 = another player, 1 = Bad AI, 2 = Better AI, 3 = Best AI)" << endl;
    cin >> p2Type;
    system("CLS");

    int boardSize = 5;
    if (p2Type != 3)
    {
        cout << "Input the size of board: (NOTE: Minimum is 3, Maximum 15, but boards from 5-12 are best)" << endl;
        cin >> boardSize;
        system("CLS");
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 15)
            boardSize = 15;
    } else
    {
        cout << "Input the size of board: (NOTE: When using Minimax the only boards are 3 or 4)" << endl;
        cin >> boardSize;
        system("CLS");
//        if (boardSize < 3)
//            boardSize = 3;
//        else if (boardSize > 4)
//            boardSize = 4;
    }
    //endregion

    //region Game Setup
    Board *board = new Board(boardSize);
    Player *p1 = new HumanPlayer(1, "Crosses (X)");
    Player *p2;

    switch (p2Type)
    {
        case 0:
        {
            p2 = new HumanPlayer(-1, "Naughts (O)");
            break;
        }
        case 1:
        {
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
        case 2:
        {
            p2 = new MonteCarloPlayer(-1, "Naughts (O)");
            break;
        }
        case 3:
        {
            p2 = new MinimaxPlayer(-1, "Naughts (O)");
            break;
        }
        case 4:
        {
            p2 = new NegascoutPlayer(-1, "Naughts (O)");
            break;
        }

        default:
        {
            cout << "That was invalid so I'll choose Bad AI for you" << endl;
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
    }
    //endregion

    HexGame game(board, p1, p2);
    game.play();

    delete board;
    delete p1;
    delete p2;

    return 0;
}
