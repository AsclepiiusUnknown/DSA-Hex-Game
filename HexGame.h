/*
 * HexGame.h
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#ifndef HEXGAME_H_
#define HEXGAME_H_

class HexGame
{
private:
    Board *board;
    Player *player[2];
public:
    bool canPlay = true;

    HexGame(Board *b, Player *p1, Player *p2)
    {
        board = b;
        player[0] = p1;
        player[1] = p2;
    }

    void play();
};

void HexGame::play()
{
    int won = 0;
    board->printBoard(); //Print the game board to the screen
    board->allSpots(); //Add all of the free coordinates to the spots vector

    //Loop until someone has won or the board has been filled
    while (!won && !board->isBoardFull())
    {
        int playerType = board->getTurn();
        int playerIndex = (playerType == player[0]->getType()) ? 0 : 1;
        int x = -1;
        int y = -1;

        if (!player[playerIndex]->getMove(board, x, y))
        {
            cout << "ERROR: No available move" << endl;
            return;
        } else
            //system("CLS");

            cout << player[playerIndex]->getPlayerName() << " played ";
        board->printCoord(x + 1, y + 1, false);

        if (!board->addMove(playerType, x, y))
            return;

        board->printBoard();
        won = board->checkWinningStatus(playerType, x, y);
        if (won == playerType)
            cout << player[playerIndex]->getPlayerName() << " player wins!" << endl;
    }
}

#endif /* HEXGAME_H_ */
