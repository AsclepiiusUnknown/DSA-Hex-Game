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

int main()
{
	int boardSize = 5;
	cout <<"Input the size of board: (Minimum is 3, but boards from 5-12 are best)" << endl;
	cin >> boardSize;
	system("CLS");
	if (boardSize < 3)
		boardSize = 3;

	Board *board = new Board(boardSize);

	int p2Type = 1;
	cout <<"Who do you want to verse? (0 = another player, 1 = Bad AI, 2 = Better AI)" << endl;
	cin >> p2Type;
	system("CLS");

	Player *p1 = new HumanPlayer(1, "Crosses");
	Player *p2;

	switch(p2Type)
	{
		case 0:
			{
				p2 = new HumanPlayer(-1, "Naughts (Human)");
				break;
			}
		case 1:
			{
				p2 = new RandomPlayer(-1, "Naughts (Random AI)");
				break;
			}
		default:
			{
				p2 = new RandomPlayer(-1, "Naughts (Random AI)");
				break;
			}
	}

	HexGame game(board, p1, p2);
	game.play();

	delete board;
	delete p1;
	delete p2;

//	system("pause");
	return 0;
}
