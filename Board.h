#ifndef BOARD_H_
#define BOARD_H_

#include<vector>
#include<stack>
#include<algorithm>
#include <windows.h>
#include "Cell.h"

using namespace std;


class Board
{
private:
    int boardSize;
    int turn;
    vector<Cell> emptyCells;
    vector<Cell> allCells;
public:
    Board(int bs)
    {
        boardSize = bs;
        grid = new int *[boardSize];
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

        grid = new int *[boardSize];
        for (int i = 0; i < boardSize; i++)
            grid[i] = new int[boardSize];

        for (int i = 0; i < boardSize; i++)
            for (int j = 0; j < boardSize; j++)
                grid[i][j] = 0;

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                grid[i][j] = cboard.getGridVal(i, j);
            }
        }

        turn = cboard.getTurn();
        emptyCells = cboard.emptyCells;
        allCells = cboard.allCells;
    }

    //region Spots
    void addCells();

    void PrintCells(vector<Cell> cells);

    //region Free Spots
    vector<Cell> getFreeCells()
    {
        return emptyCells;
    }

    int freeCellsSize()
    {
        return emptyCells.size();
    }

    void removeFreeCell(int x, int y);
    //endregion

    //region All Spots
    vector<Cell> getAllCells()
    {
        return allCells;
    }

    int allCellsSize()
    {
        return allCells.size();
    }
    //endregion

    //endregion

    int **grid;

    int getBoardSize()
    {
        return boardSize;
    }

    int getGridVal(int x, int y)
    {
        return grid[x][y];
    }

    int **getGrid()
    {
        return grid;
    }

    int getTurn()
    {
        return turn;
    }

    bool setTurn(int playerType)
    {
        turn = playerType;
        return true;
    }

    bool validInput(int, int);

    bool AddMove(int playerIndex, int x, int y);

    bool CheckForWin(int playerType, int x, int y);

    bool CheckLine(int playerType);

    bool CheckDFS(int playerType, int x, int y);

    void PrintBoard();

    bool isBoardFull();

    bool isFullThisTurn();

    stack<Cell> CheckNeighbours(int player, int x, int y);

    void PrintNeighbours(stack<Cell> s);

    void printCoord(int x, int y, bool el);

    bool isInVector(vector<Cell> v, Cell e);

    bool AddTestMove(int playerIndex, int x, int y);
};

void Board::addCells()
{
    for (int x = 0; x < boardSize; x++)
        for (int y = 0; y < boardSize; y++)
        {
            Cell cell(x, y);
            emptyCells.push_back(cell);
            allCells.push_back(cell);
        }
}

void Board::PrintCells(vector<Cell> cells)
{
    int x, y;
    int tally = 0;
    for (int i = 0; i < cells.size(); i++)
    {
        x = cells[i].x + 1, y = cells[i].y + 1;
        if (x > tally)
        {
            tally = x;
            cout << endl;
        }
        else
        {
            cout << ", ";
        }
        printCoord(x, y, false);
    }

    cout << endl;
}

void Board::removeFreeCell(int x, int y)
{
    for (int i = 0; i < emptyCells.size(); i++)
    {
        if (emptyCells[i].x == x && emptyCells[i].y == y)
        {
            emptyCells.erase(emptyCells.begin() + i);
            break;
        }
    }
}

bool Board::validInput(int x, int y)
{
    if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
    {
        cout << "Invalid Move. Node (" << x + 1 << ", " << y + 1 << ") out of range!" << endl;
        return false;
    }

    if (grid[x][y] != 0)
    {
        cout << "Invalid move. The cell (" << x + 1 << ", " << y + 1 << ") has been occupied." << endl;
        return false;
    }

    return true;
}

bool Board::AddMove(int playerIndex, int x, int y)
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
    removeFreeCell(x, y);

    turn = -1 * turn;
    return true;
}

bool Board::AddTestMove(int playerIndex, int x, int y)
{
    if (!validInput(x, y))
    {
        printf("ERROR: Invalid input used within Board's Add Test Move function\n");
        return false;
    }

    grid[x][y] = playerIndex;
    removeFreeCell(x, y);

    return true;
}

void Board::PrintBoard()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << endl;

    //SECTION - Top numbers (X)
    cout << "    ";
    for (int j = 0; j < boardSize; j++)
    {
        if (j < 9)
        {
            cout << "_";
        }
        SetConsoleTextAttribute(hConsole, 12);//RED
        cout << j + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << "_.";
    }
    cout << endl;

    //SECTION - GRID
    for (int i = 0; i < boardSize; i++)
    {
        //SECTION - Left Spacing
        for (int k = 0; k < i; k++)
            cout << "  ";

        //SECTION - Left numbers ())
        if (i < 9)
            cout << " ";
        SetConsoleTextAttribute(hConsole, 9);//BLUE
        cout << i + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << " ";

        //SECTION - Loop through all cells
        for (int j = 0; j < boardSize; j++)
        {
            //SECTION - Empty Cells
            if (grid[i][j] == 0)
            {
                if (j == 0)
                {
                    cout << "|___|";
                }
                else
                {
                    cout << "___|";
                }
            }
                //SECTION - X Cells
            else if (grid[i][j] == 1)
            {
                //First cell
                if (j == 0)
                {
                    cout << "|_";
                    SetConsoleTextAttribute(hConsole, 12);//RED
                    cout << "X";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
                    //All other Cells
                else
                {
                    cout << "_";
                    SetConsoleTextAttribute(hConsole, 12);//RED
                    cout << "X";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
            }
                //SECTION - O Cells
            else
            {
                //First cell
                if (j == 0)
                {
                    cout << "|_";
                    SetConsoleTextAttribute(hConsole, 9);//BLUE
                    cout << "O";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
                    //All other Cells
                else
                {
                    cout << "_";
                    SetConsoleTextAttribute(hConsole, 9);//BLUE
                    cout << "O";
                    SetConsoleTextAttribute(hConsole, 15);//WHITE
                    cout << "_|";
                }
            }
        }

        //SECTION - Close End Boxes & Right Numbers (O)
        if (i < boardSize - 1)
        {
            cout << "_";
            SetConsoleTextAttribute(hConsole, 9);//BLUE
            cout << i + 1;
            SetConsoleTextAttribute(hConsole, 15);//WHITE
        }
        else
        {
            cout << " ";
            SetConsoleTextAttribute(hConsole, 9);//BLUE
            cout << i + 1;
            SetConsoleTextAttribute(hConsole, 15);//WHITE
        }
        cout << endl;
    }

    //SECTION - Bottom Numbers
    for (int s = 0; s < boardSize; s++)
        cout << "  ";
    cout << "  ";
    for (int e = 0; e < boardSize; e++)
    {
        if (e < 9)
        {
            cout << " ";
        }
        SetConsoleTextAttribute(hConsole, 12);//RED
        cout << e + 1;
        SetConsoleTextAttribute(hConsole, 15);//WHITE
        cout << "  ";
    }
    cout << endl;
}

bool Board::isBoardFull()
{
    if (emptyCells.size() > 0)
        return false;

    cout << "Woops! The board is full and no one has won, looks like it's game over." << endl;
    return true;
}

bool Board::isFullThisTurn()
{
    int count = 0;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (grid[i][j] == 0)
                ++count;
        }
    }

    if (count == 1)
    {
        cout << "Yikes, looks like there's only one free spot. Better make this move count or its game over." << endl;
        return true;
    }
    else
        return false;
}

bool Board::CheckForWin(int playerType, int x, int y)
{
    if ((emptyCells.size() + (boardSize * 2 - 1)) <= (boardSize * boardSize))
    {
        if (CheckLine(playerType) || CheckDFS(playerType, x, y))
            return true;
    }
    return false;
}

bool Board::CheckLine(int playerType)
{
    bool line;

    //Check if a whole row is complete (for O)
    if (playerType == -1)
        for (int r = 0; r < boardSize; r++)
        {
            line = true;

            for (int c = 0; c < boardSize - 1; c++)
            {
                if (grid[r][c] != grid[r][c + 1] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    //Check if a whole column is complete (for X)
    if (playerType == 1)
        for (int c = 0; c < boardSize; c++)
        {
            line = true;

            for (int r = 0; r < boardSize - 1; r++)
            {
                if (grid[r][c] != grid[r + 1][c] || grid[r][c] != playerType)
                {
                    line = false;
                    break;
                }
            }
            if (line)
                return true;
        }

    for (int i = 0; i < boardSize - 1; i++)
    {
        if (grid[i][boardSize - 1 - i] != grid[i + 1][boardSize - 2 - i] || grid[i][boardSize - 1 - i] != playerType)
        {
            return false;
        }
    }

    return true;
}

bool Board::CheckDFS(int playerType, int x, int y) //DFS
{
    stack<Cell> search = CheckNeighbours(playerType, x, y);
    vector<Cell> visited;
    if (search.empty())
        return false;

    bool start = false, finish = false;
    int startGoal = 0, endGoal = boardSize - 1;

    while (!search.empty())
    {
        Cell s = search.top();
        search.pop();
        visited.push_back(s);

        if (playerType == -1)
        {
            if (s.y == startGoal)
                start = true;
            else if (s.y == endGoal)
                finish = true;
        }
        else if (playerType == 1)
        {
            if (s.x == startGoal)
                start = true;
            else if (s.x == endGoal)
                finish = true;
        }

        if (start && finish)
        {
            return true;
        }
        else
        {
            stack<Cell> children = CheckNeighbours(playerType, s.x, s.y);
            while (!children.empty())
            {
                if (!isInVector(visited, children.top()))
                    search.push(children.top());
                children.pop();
            }
        }
    }
    return false;
}

stack<Cell> Board::CheckNeighbours(int player, int x, int y)
{
    stack<Cell> neighbours;
    Cell value(0, 0);

    //Left
    if ((y - 1) >= 0)
        if (grid[x][y - 1] == player)
        {
            value.x = x, value.y = y - 1;
            neighbours.push(value);
        }

    //Right
    if ((y + 1) < boardSize)
        if (grid[x][y + 1] == player)
        {
            value.x = x, value.y = y + 1;
            neighbours.push(value);
        }

    //Up-Left
    if ((x - 1) >= 0)
        if (grid[x - 1][y] == player)
        {
            value.x = x - 1, value.y = y;
            neighbours.push(value);
        }

    //Up-Right
    if ((x - 1) >= 0 && (y + 1) < boardSize)
        if (grid[x - 1][y + 1] == player)
        {
            value.x = x - 1, value.y = y + 1;
            neighbours.push(value);
        }

    //Down-Left
    if ((x + 1) < boardSize && (y - 1) >= 0)
        if (grid[x + 1][y - 1] == player)
        {
            value.x = x + 1, value.y = y - 1;
            neighbours.push(value);
        }

    //Down-Right
    if ((x + 1) < boardSize)
        if (grid[x + 1][y] == player)
        {
            value.x = x + 1, value.y = y;
            neighbours.push(value);
        }

    return neighbours;
}

void Board::PrintNeighbours(stack<Cell> s)
{
    if (s.empty())
        return;

    Cell t = s.top();
    s.pop();

    PrintNeighbours(s);

    printCoord(s.top().x + 1, s.top().y + 1, false);
    cout << ", ";
}

bool Board::isInVector(vector<Cell> v, Cell e)
{
    if (v.empty())
        return false;

    for (Cell i : v)
        if (i.x == e.x && i.y == e.y)
            return true;

    return false;
}

void Board::printCoord(int x, int y, bool el)
{
    cout << "(" << x << ", " << y << ")";
    if (el)
        cout << endl;
}

#endif /* BOARD_H_ */