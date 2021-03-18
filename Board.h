/*
 * BaseBoard.h
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#ifndef BOARD_H_
#define BOARD_H_

class Board
{
private:
    int boardSize;
    int turn;
    int **grid;
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

    bool addMove(int playerType, int x, int y);

    int checkWinningStatus(int playerType)
    {
        //To be implemented
        return 0;
    }

    void printBoard();
};

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

bool Board::addMove(int playerType, int x, int y)
{

    if (playerType != turn)
    {
        cout << "It is not the player's turn!" << endl;
        return false;
    }

    if (grid[x][y] != 0)
    {
        cout << "Invalid move. The cell has been occupied." << endl;
        return false;
    }

    grid[x][y] = playerType;

    turn = -1 * turn;
    return true;
}

void Board::printBoard()
{
    //Top X numbers
    cout << "   ";
    for (int j = 0; j < boardSize; j++)
    {
        if (j < 9)
            cout << "_" << j + 1 << "_.";
        else
            cout << "_" << j + 1 << ".";
    }
    cout << endl;

    //RHS Y numbers & grid
    for (int i = 0; i < boardSize; i++)
    {
        for (int k = 0; k < i; k++)
            cout << " ";

        if (i < 9)
            cout << " " << i + 1 << " ";
        else
            cout << i + 1 << " ";

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
        if(i == boardSize/2 || i == boardSize/2+0.5)
            cout << "   Y Goal";
        cout << endl;
    }


    for(int s = 0; s < boardSize; s++)
        cout << " ";

    cout << "   ";
    for(int e = 0; e < boardSize; e++)
    {
        if(e < 9)
            cout << " " << e + 1 << "  ";
        else
            cout << " " << e + 1 << " ";
    }
    cout << endl;

    for(int g = 0; g < boardSize*3; g++)
        cout << " ";
    cout << " X Goal" << endl;
}

#endif /* BOARD_H_ */
