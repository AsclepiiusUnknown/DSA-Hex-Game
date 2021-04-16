#ifndef BOARD_CXX
#define BOARD_CXX

#include "Board.h"

void Board::addCells()
{
    for (int r = 0; r < boardSize; r++)
    {
        for (int c = 0; c < boardSize; c++)
        {
            Cell cell(r, c);
            emptyCells.push_back(cell);
        }
    }
}

void Board::PrintCells(vector <Cell> cells)
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

void Board::RemoveEmptyCell(int x, int y)
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

void Board::PrintBoard()
{
    cout << endl;

    cout << "    ";
    for (int j = 0; j < boardSize; j++)
    {
        if (j < 9)
        {
            cout << "_";
        }
        cout << j + 1;
        cout << "_.";
    }
    cout << endl;

    for (int i = 0; i < boardSize; i++)
    {
        for (int k = 0; k < i; k++)
            cout << "  ";

        if (i < 9)
            cout << " ";
        cout << i + 1;
        cout << " ";

        for (int j = 0; j < boardSize; j++)
        {
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
            else if (grid[i][j] == 1)
            {
                if (j == 0)
                {
                    cout << "|_";
                    cout << "X";
                    cout << "_|";
                }
                else
                {
                    cout << "_";
                    cout << "X";
                    cout << "_|";
                }
            }
            else
            {
                if (j == 0)
                {
                    cout << "|_";
                    cout << "O";
                    cout << "_|";
                }
                else
                {
                    cout << "_";
                    cout << "O";
                    cout << "_|";
                }
            }
        }

        if (i < boardSize - 1)
        {
            cout << "_";
            cout << i + 1;
        }
        else
        {
            cout << " ";
            cout << i + 1;
        }
        cout << endl;
    }

    for (int s = 0; s < boardSize; s++)
        cout << "  ";
    cout << "  ";
    for (int e = 0; e < boardSize; e++)
    {
        if (e < 9)
        {
            cout << " ";
        }
        cout << e + 1;
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

stack <Cell> Board::CheckNeighbours(int target, int x, int y)
{
    stack <Cell> neighbours;

    if ((y - 1) >= 0)
        if (grid[x][y - 1] == target)
        {

            neighbours.push(Cell(x, y - 1));
        }

    if ((y + 1) < boardSize)
        if (grid[x][y + 1] == target)
        {
            neighbours.push(Cell(x, y + 1));
        }

    if ((x - 1) >= 0)
        if (grid[x - 1][y] == target)
        {
            neighbours.push(Cell(x - 1, y));
        }

    if ((x - 1) >= 0 && (y + 1) < boardSize)
        if (grid[x - 1][y + 1] == target)
        {
            neighbours.push(Cell(x - 1, y + 1));
        }

    if ((x + 1) < boardSize && (y - 1) >= 0)
        if (grid[x + 1][y - 1] == target)
        {
            neighbours.push(Cell(x + 1, y - 1));
        }

    if ((x + 1) < boardSize)
        if (grid[x + 1][y] == target)
        {
            neighbours.push(Cell(x + 1, y));
        }

    return neighbours;
}

void Board::PrintNeighbours(stack <Cell> s)
{
    if (s.empty())
        return;

    Cell t = s.top();
    s.pop();

    PrintNeighbours(s);

    printCoord(t.x + 1, t.y + 1, false);
    cout << ", ";
}

bool Board::isInVector(vector <Cell> v, Cell e)
{
    if (v.empty())
        return false;

    for (Cell i : v)
        if (i.x == e.x && i.y == e.y)
            return true;

    return false;
}

bool Board::IsValidInput(int x, int y)
{
    if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
    {
        cout << "Invalid Move. ASCell (" << x + 1 << ", " << y + 1 << ") out of range!" << endl;
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
    RemoveEmptyCell(x, y);

    stack <Cell> neighbours = CheckNeighbours(playerIndex, x, y);
    if (!neighbours.empty())
    {
        cout << ", which connects to: ";
        PrintNeighbours(neighbours);
    }
    else
        cout << endl;

    turn = -1 * turn;
    return true;
}

bool Board::AddTestMove(int playerIndex, int x, int y)
{
    if (!IsValidInput(x, y))
    {
        printf("ERROR: Invalid input used within Board's Add Test Move function\n");
        return false;
    }

    grid[x][y] = playerIndex;
    RemoveEmptyCell(x, y);

    return true;
}

int Board::Evaluation(int player, int opponent)
{
    if (CanWin())
    {
        if (CheckForWin(player))
            return (player);
        if (CheckForWin(opponent))
            return (opponent);
    }

    if (EmptySize() > 0)
        return 0;

    return -5;
}

bool Board::CheckForWin(int playerType)
{
    if (CanWin())
        if (CheckLine(playerType) || DepthFirstSearch(playerType))
            return true;

    return false;
}

bool Board::CheckLine(int playerType)
{
    bool line;

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

    int diagTally = 0;
    for (int i = 0; i < boardSize - 1; i++)
    {
        if (grid[i][boardSize - 1 - i] != grid[i + 1][boardSize - 2 - i] || grid[i][boardSize - 1 - i] != playerType)
        {
            return false;
        }
        else
            diagTally++;
    }
    if (diagTally == boardSize)
        return true;

    return false;
}

bool Board::DepthFirstSearch(int playerType)
{
    bool start = false, finish = false;
    int startGoal = 0, endGoal = boardSize - 1;

    stack <Cell> search;
    vector <Cell> visited;


    for (int i = 0; i < boardSize; i++)
    {
        if (playerType == -1 && grid[i][0] == playerType) // O
            search.push(Cell(i, 0));
        else if (playerType == 1 && grid[0][i] == playerType) // X
            search.push(Cell(0, i));
    }

    if (search.empty())
        return false;

    while (!search.empty())
    {
        Cell s = search.top();
        search.pop();
        visited.push_back(s);

        if ((playerType == -1 && s.y == startGoal) || (playerType == 1 && s.x == startGoal))
        {
            start = true;
        }
        else if ((playerType == -1 && s.y == endGoal) || (playerType == 1 && s.x == endGoal))
        {
            finish = true;
        }

        if (start && finish)
        {
            return true;
        }
        else
        {
            stack <Cell> children = CheckNeighbours(playerType, s.x, s.y);
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

void Board::printCoord(int x, int y, bool el)
{
    cout << "(" << x << ", " << y << ")";
    if (el)
        cout << endl;
}

#endif // BOARD_CXX