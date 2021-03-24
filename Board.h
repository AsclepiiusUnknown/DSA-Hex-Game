/*
 * BaseBoard.h
 *
 *  Created on: 21/02/2021
 *      Author: Dongmo
 */

#ifndef BOARD_H_
#define BOARD_H_

#include<vector>

class Board
{
	private:
		int boardSize;
		int turn;
		int **grid;
		vector<int> spots;
	public:
		Board(int bs)
		{
			boardSize = bs;
			grid = new int*[boardSize];
			for (int i = 0; i < boardSize; i++)
				grid[i] = new int[boardSize];

			for (int i = 0; i < boardSize; i++)
				for (int j = 0; j < boardSize; j++)
				{
					grid[i][j] = 0;
				}
			turn = 1;
		}

		virtual ~Board()
		{

			for (int i = 0; i < boardSize; i++)
				delete[] grid[i];

			delete[] grid;
		}

		Board(Board &cboard)
		{
			boardSize = cboard.getBoardSize();

			grid = new int*[boardSize];
			for (int i = 0; i < boardSize; i++)
				grid[i] = new int[boardSize];

			for (int i = 0; i < boardSize; i++)
				for (int j = 0; j < boardSize; j++)
					grid[i][j] = 0;

			for (int i = 0; i < boardSize; i++)
				for (int j = 0; j < boardSize; j++)
				{
					grid[i][j] = cboard.getGrid(i, j);
				}

			turn = cboard.getTurn();
		}

		int getBoardSize()
		{
			return boardSize;
		}

		int getGrid(int x, int y)
		{
			return grid[x][y];
		}

		int getTurn()
		{
			return turn;
		}

		bool validInput(int, int);

		bool addMove(int playerIndex, int x, int y);

		int checkWinningStatus(int playerIndex);

		void printBoard();

		bool isBoardFull();
		bool isFullThisTurn();

		void allSpots();
		void printSpots();
		void removeSpot(int x, int y);
		vector<int> getSpots()
		{
			return spots;
		}
};

void Board::allSpots()
{
	for(int x = 0; x < boardSize; x++)
		for(int y = 0; y < boardSize; y++)
		{
			int value = x*boardSize+y;
			//cout << x << " " << boardSize << " " << y << endl;

			spots.push_back(value);

			//int i = value/boardSize+1;
			//int j = value%boardSize+1;
			//cout << i << j << endl;
		}
}

void Board::printSpots()
{
	for(int i = 0; i < spots.size(); i++)
		cout << "(" <<  (spots[i]/boardSize)+1 << "," << (spots[i]%boardSize)+1 << ")" << endl;
}

void Board::removeSpot(int x, int y)
{
	int value = x*boardSize+y;

	for(int i = 0; i < spots.size(); i++)
	{
		if(spots[i] == value)
		{
			spots.erase(spots.begin()+i);
			break;
		}
	}
}

bool Board::validInput(int x, int y)
{
	if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
	{
		cout << "Move (" << x + 1 << ", " << y + 1 << ") out of range!" << endl;
		return false;
	}

	if (grid[x][y] != 0)
	{
		cout << "Invalid move. The cell has been occupied." << endl;
		return false;
	}

	return true;
}

bool Board::addMove(int playerIndex, int x, int y)
{
	if (playerIndex != turn)
	{
		cout << "It is not the player's turn!" << endl;
		return false;
	}

	if (grid[x][y] != 0)
	{
		cout << "Invalid move. The cell has been occupied." << endl;
		return false;
	}

	grid[x][y] = playerIndex;
	removeSpot(x,y);

	turn = -1 * turn;
	return true;
}

void Board::printBoard()
{
	//Top X numbers
	cout << "   ";
	for (int j = 0; j < boardSize; j++)
	{
		if (j < 9) cout << "_" << j + 1 << "_.";
		else cout << "_" << j + 1 << ".";
	}
	cout << endl;

	//RHS Y numbers & grid
	for (int i = 0; i < boardSize; i++)
	{
		for (int k = 0; k < i; k++)
			cout << " ";

		if (i < 9) cout << " " << i + 1 << " ";
		else cout << i + 1 << " ";

		for (int j = 0; j < boardSize; j++)
		{
			if (grid[i][j] == 0)
			{
				if (j == 0)
				{
					cout << "\\___\\";
				}
				else
				{
					cout << "___\\";
				}
			}
			else if (grid[i][j] == 1)
			{
				if (j == 0)
				{
					cout << "\\_X_\\";
				}
				else
				{
					cout << "_X_\\";
				}
			}
			else
			{
				if (j == 0)
				{
					cout << "\\_O_\\";
				}
				else
				{
					cout << "_O_\\";
				}
			}
		}

		cout << " " << i + 1;
		if (i == boardSize / 2 || i == boardSize / 2 + 0.5) cout << "   Y Goal";
		cout << endl;
	}

	for (int s = 0; s < boardSize; s++)
		cout << " ";

	cout << "   ";
	for (int e = 0; e < boardSize; e++)
	{
		if (e < 9) cout << " " << e + 1 << "  ";
		else cout << " " << e + 1 << " ";
	}
	cout << endl;

	for (int g = 0; g < boardSize * 3; g++)
		cout << " ";
	cout << " X Goal" << endl;
}

bool Board::isBoardFull()
{
	if(spots.size() > 0)
		return false;

	cout << "Woops! The board is full and no one has won, looks like it's game over." << endl;
	return true;
}

bool Board::isFullThisTurn()
{
	int count = 0;
	for(int i = 0; i < boardSize; i++)
	{
		for(int j = 0; j < boardSize; j++)
		{
			if(grid[i][j] == 0)
				++count;
		}
	}

	if(count==1)
	{
		cout << "Yikes, looks like there's only one free spot. Better make this move count or its game over." << endl;
		return true;
	}
	else
		return false;
}

int Board::checkWinningStatus(int playerIndex)
{
	//Check if a whole row is complete
	for (int r=0; r<boardSize; r++)
	{
		bool line = true;
		for(int c=0; c<boardSize-1; c++)
		{
			if(grid[r][c] != grid[r][c+1] || grid[r][c] == 0)
			{
				line = false;
				break;
			}
		}
		if(line)
			return playerIndex;
	}

	//Check if a whole column is complete
	for (int c=0; c<boardSize; c++)
	{
		bool line = true;
		for(int r=0; r<boardSize-1; r++)
		{
			if(grid[r][c] != grid[r+1][c] || grid[r][c] == 0)
			{
				line = false;
				break;
			}
		}
		if(line)
			return playerIndex;
	}

	//1st diagonal is complete
	bool d1 = true;
	for(int i=0; i<boardSize; i++)
	{
		if(grid[i][i] != grid[i+1][i+1] || grid[i][i] == 0)
		{
			d1 = false;
			break;
		}
	}
	if(d1)
		return playerIndex;

	//2nd diagonal is complete
	int d2 = true;
	int bs = boardSize-1;
	for(int i=0; i<bs; i++)
	{
		if(grid[i][bs-i] != grid[i+1][bs-i-1] || grid[i][bs-i] == 0)
		{
			d2 = false;
			break;
		}
	}
	if(d2)
		return playerIndex;

	return 0;
}

#endif /* BOARD_H_ */
