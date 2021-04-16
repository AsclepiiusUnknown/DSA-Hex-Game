#ifndef HEXGAME_H_
#define HEXGAME_H_

class HexGame
{
private:
    Board *board;
    Player *player[2];
public:
    HexGame(Board *b, Player *p1, Player *p2)
    {
        board = b;
        player[0] = p1;
        player[1] = p2;
    }

    int Play(bool showAll, int startPlayer);
};

int HexGame::Play(bool showAll, int startPlayer)
{
    if (!showAll)
        system("CLS");

    bool won = false;
    board->PrintBoard();
    board->addCells();
    board->SetTurn(startPlayer);

    while (!won && !board->isBoardFull())
    {
        int playerType = board->GetTurn();
        int playerIndex = (playerType == player[0]->GetType()) ? 0 : 1;
        int x = -1;
        int y = -1;

        if (!player[playerIndex]->GetMove(board, x, y))
        {
            cout << "ERROR: No available move for ";
            board->printCoord(x + 1, y + 1, false);
            cout << endl;
            return 0;
        }

        if (!showAll)
            system("CLS");

        cout << player[playerIndex]->GetSymbol() << " played ";
        board->printCoord(x + 1, y + 1, false);

        if (!board->AddMove(playerType, x, y))
            return 0;

        board->PrintBoard();
        won = board->CheckForWin(playerType);
        if (won)
        {
            cout << player[playerIndex]->GetName() << " player wins!" << endl;
            return playerType;
        }
    }

    return 0;
}

#endif /* HEXGAME_H_ */
