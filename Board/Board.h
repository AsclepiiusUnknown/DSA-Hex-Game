#ifndef BOARD_H
#define BOARD_H

class Board
{
private:
    int **grid;
    int boardSize;
    int turn;
    vector <Cell> emptyCells{};

public:
    explicit Board(int bs)
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
        boardSize = cboard.GetBoardSize();

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
                grid[i][j] = cboard.GridValue(i, j);
            }
        }

        turn = cboard.GetTurn();
        emptyCells = cboard.emptyCells;
    }

    void addCells();

    void PrintCells(vector <Cell> cells);

    vector <Cell> GetEmpty()
    {
        return emptyCells;
    }

    int EmptySize()
    {
        return emptyCells.size();
    }

    void RemoveEmptyCell(int x, int y);


    int GetBoardSize()
    {
        return boardSize;
    }

    void PrintBoard();

    bool isBoardFull();


    int GridValue(int x, int y)
    {
        return grid[x][y];
    }

    int **GetGrid()
    {
        return grid;
    }


    stack <Cell> CheckNeighbours(int target, int x, int y);

    bool isInVector(vector <Cell> v, Cell e);

    bool isInVector(vector <PathCell> v, PathCell e);

    void PrintNeighbours(stack <Cell> s);


    bool AddTestMove(int playerIndex, int x, int y);

    bool AddMove(int playerIndex, int x, int y);

    bool IsValidInput(int x, int y);


    int Evaluation(int player, int opponent);

    bool CheckForWin(int playerType);

    bool CheckLine(int playerType);

    bool DepthFirstSearch(int playerType);


    int MoveNumber()
    { return (((boardSize * boardSize) - EmptySize()) + 1); }

    bool SetTurn(int playerType)
    {
        turn = playerType;
        return true;
    }

    bool CanWin()
    {
        if ((EmptySize() + (boardSize * 2 - 1)) <= (boardSize * boardSize))
            return true;
        return false;
    }

    void printCoord(int x, int y, bool el);

    int GetTurn()
    {
        return turn;
    }

};

#endif //BOARD_H
