/*
 * Main.cpp
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#include<iostream>
#include "Board.h"
#include "Players/Player.h"
#include "Players/HumanPlayer.h"
#include "HexGame.h"
#include "Players/RandomPlayer.h"
#include "Players/MonteCarloPlayer.h"
#include "Players/MinimaxPlayer.h"
#include "Players/MinimaxPlayer2.h"
#include "Players/NegascoutPlayer.h"

using namespace std;


int main()
{
    //region Pre-Game Input
    //SECTION - Get Input for who the player wants to verse (default is of type RandomPlayer)
    int p2Type = 1;
    cout << "Who do you want to verse? (0 = another player, 1 = Bad AI, 2 = Better AI, 3 = Best AI)" << endl;
    cin >> p2Type;
    system("CLS");

    //SECTION - Get input for the size of the board. When using Minimax, this is restricted to a board of either 3 or 4
    int boardSize = 5;
    if (p2Type == 3)
    {
        cout << "Input the size of board: (NOTE: When using Minimax the only boards are 3 or 4)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 4)
            boardSize = 4;
        system("CLS");
    }
    else
    {
        cout << "Input the size of board: (NOTE: Minimum is 3, Maximum 15, but boards from 5-12 are best)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 15)
            boardSize = 15;
        system("CLS");
    }
    //endregion

    //region Game Setup
    //SECTION - Setup the players based off of the input from above.
    Board *board = new Board(boardSize);
    Player *p1 = new HumanPlayer(1, "Crosses (X)");
    Player *p2;

    switch (p2Type)
    {
        case 0:     //Human Player 2 (not AI)
        {
            p2 = new HumanPlayer(-1, "Naughts (O)");
            break;
        }
        case 1:     //Random Player 2 (simple AI that chooses random empty position)
        {
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
        case 2:     //Monte Carlo Player 2 (Medium AI that takes samples of random positions and chooses the best one through heuristic analysis)
        {
            p2 = new MonteCarloPlayer(-1, "Naughts (O)");
            break;
        }
        case 3:     //Minimax Player 2 (Hard AI that simulates all possible moves from the current state and chooses the one that leads to the fastes win)
        {
            p2 = new MinimaxPlayer(-1, "Naughts (O)");
            break;
        }
        case 4:     //Negascout Player 2 (Experimental variation of Minimax with aimed at being more efficient whilst also being more accurate, hopefully allowing for better play on big boards
        {
            p2 = new NegascoutPlayer(-1, "Naughts (O)");
            break;
        }

        default:    //Choose Random Player 2 in the case of an error such as invalid input
        {
            cout << "That was invalid so I'll choose Bad AI for you" << endl;
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
    }
    //endregion

    //Create and begin a new game using our predetermined values
    HexGame game(board, p1, p2);
    game.play();

    delete board;
    delete p1;
    delete p2;

    return 0;
}
