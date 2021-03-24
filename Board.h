/*
 * BaseBoard.h
 *
 *  Created on: 21/02/2021
 *      Author: Dongmo
 */

#ifndef BOARD_H_
#define BOARD_H_

#include<vector>
#include<stack>
#include<algorithm>

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

		int checkWinningStatus(int playerType, int x, int y);
		int lineWin(int playerType);
		bool backtrackWin(int playerType, int x, int y);

		void printBoard();

		bool isBoardFull();
		bool isFullThisTurn();

		int CoordToInt(int x, int y)
		{
			return (x*boardSize+y);
		}
		int toX(int value)
		{
			return (value/boardSize);
		}
		int toY(int value)
		{
			return (value%boardSize);
		}

		void allSpots();
		void printSpots();
		void removeSpot(int x, int y);
		vector<int> getSpots()
		{
			return spots;
		}

		stack<int> checkNeighbours(int player, int x, int y);
		void printNeighbours(stack<int> s);

		void printCoord(int x, int y, bool el);
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

	stack<int> neighbours = checkNeighbours(playerIndex,x,y);
	if(neighbours.size() > 0)
		cout << "This move connects to:" << endl;
	printNeighbours(neighbours); //need to disable the clear screen in HexGame to see results

	printBoard();

	turn = -1 * turn;
	return true;
}

void Board::printBoard()
{
	//Top X numbers
	cout << "    ";
	for (int j = 0; j < boardSize; j++)
	{
		if (j < 9) cout << "_" << j + 1 << "_.";
		else cout << "" << j + 1 << "_.";
	}
	cout << endl;

	//RHS Y numbers & grid
	for (int i = 0; i < boardSize; i++)
	{
		for (int k = 0; k < i; k++)
			cout << "  ";

		if (i < 9) cout << " " << i + 1 << " ";
		else cout << i + 1 << " ";

		for (int j = 0; j < boardSize; j++)
		{
			if (grid[i][j] == 0)
			{
				if (j == 0)
				{
					cout << "  -  ";
				}
				else
				{
					cout << " -  ";
				}
			}
			else if (grid[i][j] == 1)
			{
				if (j == 0)
				{
					cout << "  X  ";
				}
				else
				{
					cout << " X  ";
				}
			}
			else
			{
				if (j == 0)
				{
					cout << "  O  ";
				}
				else
				{
					cout << " O  ";
				}
			}
		}

		if(i < boardSize-1)
			cout << "_" << i + 1;
		else
			cout << " " << i + 1;

		if (i == boardSize / 2 || i == boardSize / 2 + 0.5) cout << "   Y Goal";
		cout << endl;
	}

	for (int s = 0; s < boardSize; s++)
		cout << "  ";

	cout << " ";
	for (int e = 0; e < boardSize; e++)
	{
		if (e < 9) cout << "  " << e + 1 << " ";
		else cout << "  " << e + 1 << "";
	}
	cout << endl;

	//Bottom Border
//	cout << "|";
//	for(int b=0; b<boardSize; b++)
//    {
//        cout << "_______";
//    }

	//Bottom Goal
	for (int g = 0; g < boardSize; g++)
		cout << "  ";
	cout << " X Goal" << endl << endl;
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

int Board::checkWinningStatus(int playerType, int x, int y)
{
	if(lineWin(playerType)!=0 || backtrackWin(playerType, x, y)!=0)
		return playerType;

	return 0;
}

int Board::lineWin(int playerType)
{
	//Check if a whole row is complete (Y goal)
	if(playerType == -1)
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
				return playerType;
		}

	//Check if a whole column is complete (X goal)
	if(playerType == 1)
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
				return playerType;
		}
}

bool Board::backtrackWin(int playerType, int x, int y)
{
//	vector<int> visited;
//	stack<int> s;
//	s.push(node1);
//	unsigned int currnode;
//
//	while (!s.empty())
//	{
//		currnode = s.top();
//		s.pop(); // Pop the top node off the stack
//		visited[currnode] = true; // Set node 1 as visited
//		unsigned int node_list_size = nodes[currnode].size();
//		unsigned int candidate_node;
//		for (unsigned int i = 0; i < node_list_size; i++)   // For each node adjacent to node 1
//		{
//			candidate_node = nodes[currnode][i];
//			if (candidate_node == node2) return true; // Search is over
//			if (visited[candidate_node]) continue;
//			if (candidate_node >= size - 4 || board->getSpace(candidate_node) != color)
//			{
//				visited[candidate_node] = true;
//				continue; // If it's the wrong color, or a pseudonode, don't pathfind through it.
//			}
//			s.push(nodes[currnode][i]); // Push the new node onto the search stack
//		}
//	}
//	return false;
////_________________________________________________________________________________________________________________________
//
//	if((playerType==1 && (x==0 || x==boardSize-1)) || (playerType==-1 && (y==0 || y==boardSize-1)))
//	{
//		stack<int> trackStack = checkNeighbours(playerType,x,y);
//		vector<int> visitedStack(boardSize*boardSize, false);
//
//		while(!trackStack.empty())
//		{
//			int s = trackStack.top();
//			trackStack.pop();
//			visitedStack.push_back(s);
//
//			int sX = s/boardSize;
//			int sY = s%boardSize;
//
//			if(s == 0)  //!implement check if s is goal
//			{
//				return true;
//			}
//			else
//			{
//				stack<int> children = checkNeighbours(playerType, sX, sY);
//				while(!children.empty())
//				{
//					if(find(visitedStack.begin(), visitedStack.end(), children.top()) != visitedStack.end()) //!contains or for loop??
//						continue;
//					else
//						trackStack.push(children.top()); //!add branch marker?
//				}
//			}
//		}
//		return 0;
//	}
//	else
		return 0;
}

stack<int> Board::checkNeighbours(int player, int x, int y)
{
	stack<int> neighbours;
	int value;

	//Left
	if((y-1) >= 0)
		if(grid[x][y-1] == player)
		{
			value = CoordToInt(x,(y-1));
			neighbours.push(value);
		}

	//Right
	if((y+1) < boardSize)
		if(grid[x][y+1] == player)
		{
			value = CoordToInt(x,(y+1));
			neighbours.push(value);
		}

	//Up-Left
	if((x-1) >= 0)
		if(grid[x-1][y] == player)
		{
			value = CoordToInt((x-1),y);
			neighbours.push(value);
		}

	//Up-Right
	if((x-1) >= 0 && (y+1) < boardSize)
		if(grid[x-1][y+1] == player)
		{
			value = CoordToInt((x-1),(y+1));
			neighbours.push(value);
		}

	//Down-Left
	if((x+1) < boardSize && (y-1) >= 0)
		if(grid[x+1][y-1] == player)
		{
			value = CoordToInt((x+1),(y-1));
			neighbours.push(value);
		}

	//Down-Right
	if((x+1) < boardSize)
		if(grid[x+1][y] == player)
		{
			value = CoordToInt((x+1),y);
			neighbours.push(value);
		}

	return neighbours;
}

void Board::printNeighbours(stack<int> s)
{
	if (s.empty())
		return;

	int t = s.top();

	s.pop();

	printNeighbours(s);

	int x, y;
	x = (t/boardSize)+1;
	y = (t%boardSize)+1;
	cout << " - ";
	printCoord(x,y,true);
	cout << endl;

	//s.push(t); //adds elements back to the stack in correct order
}

void Board::printCoord(int x, int y, bool el)
{
	if(el)
		cout << "(" <<  x << "," << y << ")" << endl;
	else
		cout << "(" <<  x << "," << y << ")";
}

#endif /* BOARD_H_ */
