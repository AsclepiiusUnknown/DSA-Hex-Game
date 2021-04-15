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

    int play(bool showAll, int startPlayer);
};

int HexGame::play(bool showAll, int startPlayer)
{
    if (!showAll)
        system("CLS");

    bool won = false; //By default, no one has won
    board->PrintBoard(); //Print the game board to the screen
    board->addCells(); //Initialise the list of free spots
    board->setTurn(startPlayer);

    //Loop until someone has won or the board has been filled
    while (!won && !board->isBoardFull())
    {
        int playerType = board->getTurn(); //Determine who's turn it is
        int playerIndex = (playerType == player[0]->getType()) ? 0 : 1;
        int x = -1;
        int y = -1;

        //Get the coordinates of the move from the appropriate player
        if (!player[playerIndex]->GetMove(board, x, y))
        {
            //Debug if we are unable to make the move due to an error or full board
            cout << "ERROR: No available move for ";
            board->printCoord(x + 1, y + 1, false);
            cout << endl;
            return 0;
        }

        if (!showAll)
            system("CLS");

        //Display the move that was made
        cout << player[playerIndex]->getPlayerSymbol() << " played ";
        board->printCoord(x + 1, y + 1, false);

        //Add the move to our board
        if (!board->AddMove(playerType, x, y))
            return 0;

        board->PrintBoard(); //Print the new board (old board + this move)
        won = board->CheckForWin(playerType); //Check to see if this player has just won. If so, end the game and tell the players
        if (won)
        {
            cout << player[playerIndex]->getPlayerName() << " player wins!" << endl;
            return playerType;
        }
    }

    return 0;
}

#endif /* HEXGAME_H_ */
