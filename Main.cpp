//region References
using namespace std;

#include "chrono"
#include <cstdlib>
#include <iostream>
#include <queue>
#include <iomanip>
#include <string>
#include "vector"
#include "time.h"
#include<random>
#include<stack>
#include<algorithm>

#include "Cell.h"
#include "Board.h"
#include "Players/Player.h"
#include "HexGame.h"
#include "Players/MinimaxPlayer.h"
#include "Players/AStarPlayer.h"
#include "Players/HumanPlayer.h"
#include "Players/RandomPlayer.h"
#include "Players/MonteCarloPlayer.h"
//endregion

struct Setup
{
    Player *p1;
    Player *p2;
    Board *board;

    int times = 1;
    double p1Accuracy = 0;
    double p2Accuracy = 0;
};

struct Stats
{
    vector<int> wins;
    vector<int> turnsTaken;
    vector<int> durations;
    string p1Name;
    string p2Name;
    int boardsize;
    double p1Accuracy;
    double p2Accuracy;
};

Setup Human();

Setup Simulation();

void PrintResults(Stats stats);

int main()
{
    RESTART:
    srand(time(NULL));

    //region Pre-Game Input
    //SECTION - AI Simulation Check
    string yesNo;
    Setup setup;
    cout << "Do you want to run an AI Simulation? [y/n] (use capitals if you want to see all moves)" << endl;
    cin >> yesNo;
    if (yesNo == "Y" || yesNo == "y")
    {
        system("CLS");
        setup = Simulation();
    }
    else if (yesNo == "N" || yesNo == "n")
    {
        system("CLS");
        setup = Human();
    }
    else
    {
        system("CLS");
        cout << "Invalid input so I'll run a normal game for you." << endl;
        setup = Human();
    }

    bool showAll;
    if (yesNo == "Y" || yesNo == "N")
        showAll = true;
    else
        showAll = false;

    Stats stats;
    stats.p1Name = setup.p1->getPlayerName();
    stats.p2Name = setup.p2->getPlayerName();
    stats.boardsize = setup.board->getBoardSize();
    stats.p1Accuracy = setup.p1Accuracy;
    stats.p2Accuracy = setup.p2Accuracy;

    int startingPlayer = 1;
    while (setup.times > 0)
    {
        Board *currentBoard = new Board(setup.board->getBoardSize());
        //Create and begin a new game using our predetermined values
        HexGame game(currentBoard, setup.p1, setup.p2);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        int winner = game.play(showAll, startingPlayer);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        if (winner == 1 || winner == -1)
            stats.wins.push_back(winner);
        else
        {
            printf("FATAL ERROR: No winner was found in main()");
            return 0;
        }

        stats.turnsTaken.push_back((currentBoard->getBoardSize() * currentBoard->getBoardSize()) - currentBoard->freeCellsSize());
        stats.durations.push_back(static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()));

        if (setup.times <= 1) //If this is meant to be the last round check if they want to play another
        {
            if (!showAll)
                system("CLS");

            PrintResults(stats);

            string yesNo;
            cout << "Do you want to play again? [Y]es | [N]o | [R]estart program" << endl;
            cin >> yesNo;
            if (!showAll)
                system("CLS");

            if (yesNo == "Y" || yesNo == "y")
                setup.times++;
            else if (yesNo == "N" || yesNo == "n")
                cout << "Okay, bye." << endl;
            else if (yesNo == "R")
            {
                system("CLS");
                goto RESTART;
            }
            else
                cout << "Invalid input so I'll take that as a no. Bye." << endl;
        }
        setup.times--;
        startingPlayer *= (-1);
    }

    delete setup.board;
    delete setup.p1;
    delete setup.p2;

    return 0;
}

Setup Human()
{
    //region Pre-Human Input
    //SECTION - Get Input for who the player wants to verse (default is of type RandomPlayer)
    int p2Type = 1;
    cout << "Who do you want to verse? (0 = another player, 1 = Random AI, 2 = Monte-Carlo AI, 3 = Minimax AI)" << endl;
    cin >> p2Type;
    system("CLS");

    //SECTION - Monte-Carlo AI Accuracy Input
    double accuracy = 2.5;
    if (p2Type == 2 || p2Type == 3)
    {
        cout << "How accurate do you want the AI to be? (Between 1 and 10)" << endl;
        cin >> accuracy;
        if (accuracy <= 0)
            accuracy = 1;
        else if (accuracy > 10)
            accuracy = 10;
        system("CLS");
    }

    //SECTION - Get input for the size of the board. When using Minimax, this is restricted to a board of either 3 or 4
    int boardSize = 5;
    if (p2Type == 3)
    {
        cout << "Input the size of board: (NOTE: When using Minimax the only boards are 3, 4, or 5)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 5)
            boardSize = 5;
        system("CLS");
    }
    else
    {
        cout << "Input the size of board: (NOTE: Minimum is 3, Maximum 15, but boards from 5-12 are best)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 15)
            boardSize = 15;
        system("CLS");
    }
    //endregion

    //region Human Setup
    //SECTION - Setup the players based off of the input from above.
    Board *board = new Board(boardSize);
    Player *p1 = new HumanPlayer(1, "Crosses (X)");
    Player *p2;

    switch (p2Type)
    {
        case 0:     //Human Player 2 (not AI)
        {
            p2 = new HumanPlayer(-1, "Naughts (O)");
            break;
        }
        case 1:     //Random Player 2 (simple AI that chooses random empty position)
        {
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
        case 2:     //Monte Carlo Player 2 (Medium AI that takes samples of random positions and chooses the best one through heuristic analysis)
        {
            accuracy *= 1000;
            p2 = new MonteCarloPlayer(-1, "Naughts (O)", accuracy);
            break;
        }
        case 3:     //Minimax Player 2 (Hard AI that simulates all possible moves from the current state and chooses the one that leads to the fastes win)
        {
            p2 = new MinimaxPlayer(-1, "Naughts (O)", static_cast<double>(accuracy) * (0.15 * boardSize));
            break;
        }
        case 4:     //Negascout Player 2 (Experimental variation of Minimax with aimed at being more efficient whilst also being more accurate, hopefully allowing for better play on big boards
        {
            p2 = new AStarPlayer(-1, "Naughts (O)");
            break;
        }

        default:    //Choose Random Player 2 in the case of an error such as invalid input
        {
            cout << "That was invalid so I'll choose Bad AI for you" << endl;
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
    }
    //endregion

    Setup setup;
    setup.p1 = p1;
    setup.p2 = p2;
    setup.board = board;
    setup.p2Accuracy = accuracy;

    return setup;
}

Setup Simulation()
{
    //region Pre-Human Input
    //SECTION - Get Input for player 1 (default is of type RandomPlayer)
    int p1Type = 1;
    cout << "Who do you want as player 1? (1 = Bad AI, 2 = Better AI, 3 = Best AI)" << endl;
    cin >> p1Type;
    system("CLS");


    //SECTION - Get Input for player 2 (default is of type RandomPlayer)
    int p2Type = 1;
    cout << "Who do you want as player 2? (1 = Bad AI, 2 = Better AI, 3 = Best AI)" << endl;
    cin >> p2Type;
    system("CLS");

    //SECTION - AI1 Accuracy Input
    double accuracy1 = 2.5;
    if (p1Type == 2 || p1Type == 3)
    {
        cout << "How accurate do you want the player 1 AI to be? (Between 1 and 10)" << endl;
        cin >> accuracy1;
        if (accuracy1 <= 0)
            accuracy1 = 1;
        else if (accuracy1 > 10)
            accuracy1 = 10;
        system("CLS");
    }

    //SECTION - AI2 Accuracy Input
    double accuracy2 = 2.5;
    if (p2Type == 2 || p2Type == 3)
    {
        cout << "How accurate do you want the player 2 AI to be? (Between 1 and 10)" << endl;
        cin >> accuracy2;
        if (accuracy2 <= 0)
            accuracy2 = 1;
        else if (accuracy2 > 10)
            accuracy2 = 10;
        system("CLS");
    }

    //SECTION - Get input for the size of the board
    int boardSize = 10;
    if (p2Type == 3 || p1Type == 3)
    {
        cout << "Input the size of board: (NOTE: When using Minimax the only boards are 3, 4, or 5)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 5)
            boardSize = 5;
        system("CLS");
    }
    else
    {
        cout << "How big do you want the board? (NOTE: Minimum is 3, Maximum 15, but boards from 5-12 are best)" << endl;
        cin >> boardSize;
        if (boardSize < 3)
            boardSize = 3;
        else if (boardSize > 15)
            boardSize = 15;
        system("CLS");
    }

    //SECTION - Get No. of times to run the Simulation
    int times = 10;
    cout << "How many times did you want to run the Simulation? (Between 1 and 100)" << endl;
    cin >> times;
    if (boardSize < 1)
        boardSize = 1;
    else if (boardSize > 100)
        boardSize = 100;
    system("CLS");
    //endregion

    //region Human Setup
    //SECTION - Setup the players based off of the input from above.
    Board *board = new Board(boardSize);
    Player *p1;
    Player *p2;

    switch (p1Type)
    {
        case 1:     //Random Player 1 (simple AI that chooses random empty position)
        {
            p1 = new RandomPlayer(1, "Crosses (X)");
            break;
        }
        case 2:     //Monte Carlo Player 1 (Medium AI that takes samples of random positions and chooses the best one through heuristic analysis)
        {
            p1 = new MonteCarloPlayer(1, "Crosses (X)", accuracy1 * 1000);
            break;
        }
        case 3:     //Minimax Player 1 (Hard AI that simulates all possible moves from the current state and chooses the one that leads to the fastes win)
        {
            p1 = new MinimaxPlayer(1, "Crosses (X)", static_cast<double>(accuracy1) * (0.15 * boardSize));
            break;
        }
        case 4:     //Negascout Player 1 (Experimental variation of Minimax with aimed at being more efficient whilst also being more accurate, hopefully allowing for better play on big boards
        {
            p1 = new AStarPlayer(1, "Crosses (X)");
            break;
        }

        default:    //Choose Random Player 1 in the case of an error such as invalid input
        {
            cout << "That was invalid so I'll choose Bad AI for you" << endl;
            p1 = new RandomPlayer(1, "Crosses (X)");
            break;
        }
    }

    switch (p2Type)
    {
        case 1:     //Random Player 2 (simple AI that chooses random empty position)
        {
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
        case 2:     //Monte Carlo Player 2 (Medium AI that takes samples of random positions and chooses the best one through heuristic analysis)
        {
            p2 = new MonteCarloPlayer(-1, "Naughts (O)", accuracy2 * 1000);
            break;
        }
        case 3:     //Minimax Player 2 (Hard AI that simulates all possible moves from the current state and chooses the one that leads to the fastes win)
        {
            p2 = new MinimaxPlayer(-1, "Naughts (O)", static_cast<double>(accuracy2) * (1.0 / boardSize) * (4 * 0.3));
            break;
        }
        case 4:     //Negascout Player 2 (Experimental variation of Minimax with aimed at being more efficient whilst also being more accurate, hopefully allowing for better play on big boards
        {
            p2 = new AStarPlayer(-1, "Naughts (O)");
            break;
        }

        default:    //Choose Random Player 2 in the case of an error such as invalid input
        {
            cout << "That was invalid so I'll choose Bad AI for you" << endl;
            p2 = new RandomPlayer(-1, "Naughts (O)");
            break;
        }
    }
    //endregion

    Setup setup;
    setup.p1 = p1;
    setup.p2 = p2;
    setup.board = board;
    setup.times = times;
    setup.p1Accuracy = accuracy1;
    setup.p2Accuracy = accuracy2;

    return setup;
}

void PrintResults(Stats stats)
{
    //region Setup
    //HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    double n = 1;
    int colWidth = 20;
    //table header
    cout << endl << setfill('*') << setw(3 * colWidth) << "*" << endl;
    cout << setfill(' ') << fixed;

    cout << setw(colWidth) << "" << setw(colWidth);
    //SetConsoleTextAttribute(hConsole, 12);//RED
    cout << "X Results" << setw(colWidth);
    //SetConsoleTextAttribute(hConsole, 9);//BLUE
    cout << "O Results" << endl;
    //SetConsoleTextAttribute(hConsole, 15);//WHITE

    cout << setfill('~') << setw(3 * colWidth) << "~" << endl;
    cout << setfill(' ') << fixed;
    //endregion

    //region Data
    //SECTION - Player Type
    cout << setprecision(0) << setw(colWidth) << "PLAYER INFORMATION:" << setprecision(4) << " " << setw(colWidth) << " " << setw(colWidth) << endl;
    cout << setprecision(0) << setw(colWidth) << "Player Type" << setprecision(4) << setw(colWidth) << stats.p1Name << setw(colWidth) << stats.p2Name << endl;

    //SECTION - Accuracy
    if (stats.p1Accuracy == 0 && stats.p2Accuracy == 0)
        cout << setprecision(0) << setw(colWidth) << "Preset Accuracy" << setprecision(4) << setw(colWidth) << "N/A" << setw(colWidth) << "N/A" << endl;
    else if (stats.p1Accuracy == 0)
        cout << setprecision(0) << setw(colWidth) << "Preset Accuracy" << setprecision(4) << setw(colWidth) << "N/A" << setw(colWidth) << stats.p2Accuracy << endl;
    else if (stats.p2Accuracy == 0)
        cout << setprecision(0) << setw(colWidth) << "Preset Accuracy" << setprecision(4) << setw(colWidth) << stats.p1Accuracy << setw(colWidth) << "N/A" << endl;
    else
        cout << setprecision(0) << setw(colWidth) << "Preset Accuracy" << setprecision(4) << setw(colWidth) << stats.p1Accuracy << setw(colWidth) << stats.p2Accuracy << endl;

    //region WIN INFO
    cout << setfill('~') << setw(3 * colWidth) << "~" << endl;
    cout << setfill(' ') << fixed;
    cout << setprecision(0) << setw(colWidth) << "WIN INFORMATION:" << setprecision(4) << " " << setw(colWidth) << " " << setw(colWidth) << endl;

    //Loop through all the wins, counting for each side and making a list of the winning indicies (for later)
    vector<int> xIndices, oIndices;
    int xWins = 0, oWins = 0;
    for (int i = 0; i < stats.wins.size(); i++)
    {
        if (stats.wins[i] == 1)
        {
            xWins++;
            xIndices.push_back(i);
        }
        else
        {
            oWins++;
            oIndices.push_back(i);
        }
    }

    //SECTION - Win-Count
    cout << setprecision(0) << setw(colWidth) << "No. of Wins" << setprecision(4) << setw(colWidth) << xWins << setw(colWidth) << oWins << endl;

    //SECTION - Win Percent
    int xPct = round((static_cast<double>(xWins) / stats.wins.size()) * 100);
    int oPct = round((static_cast<double>(oWins) / stats.wins.size()) * 100);
    string xPctString = std::to_string(xPct) + "%";
    string oPctString = std::to_string(oPct) + "%";
    cout << setprecision(0) << setw(colWidth) << "% of Wins" << setprecision(4) << setw(colWidth) << xPctString << setw(colWidth) << oPctString << endl;
    //endregion

    //region TURN INFO
    cout << setfill('~') << setw(3 * colWidth) << "~" << endl;
    cout << setfill(' ') << fixed;
    cout << setprecision(0) << setw(colWidth) << "TURN INFORMATION:" << setprecision(4) << " " << setw(colWidth) << " " << setw(colWidth) << endl;

    int xTurns = 0, oTurns = 0;
    vector<int> xTurnsTaken, oTurnsTaken;

    //SECTION - Turns for each round
    //Loop through the Turns and print the turns for each round
    for (int i = 0; i < stats.turnsTaken.size(); i++)
    {
        if (stats.turnsTaken[i] % 2 == 0)   //Check if its an even number
        {
            xTurns += (stats.turnsTaken[i] / 2);
            oTurns += (stats.turnsTaken[i] / 2);
            cout << setprecision(0) << setw(colWidth) << "Round " + to_string(i + 1) + " Turns" << setprecision(4) << setw(colWidth) << stats.turnsTaken[i] / 2 << setw(colWidth)
                 << stats.turnsTaken[i] / 2 << endl;
            xTurnsTaken.push_back(stats.turnsTaken[i] / 2);
            oTurnsTaken.push_back(stats.turnsTaken[i] / 2);
        }
        else //If not, then split the odd accordingly
        {
            int halfway = round(stats.turnsTaken[i] / 2);
            xTurns += stats.turnsTaken[i] - halfway;
            oTurns += halfway;
            cout << setprecision(0) << setw(colWidth) << "Round " + to_string(i + 1) + " Turns" << setprecision(4) << setw(colWidth) << stats.turnsTaken[i] - halfway << setw(colWidth) << halfway
                 << endl;
            xTurnsTaken.push_back(stats.turnsTaken[i] - halfway);
            oTurnsTaken.push_back(halfway);
        }
    }

    //SECTION - Total Number or Turns
    cout << setprecision(0) << setw(colWidth) << "Total Turns" << setprecision(4) << setw(colWidth) << xTurns << setw(colWidth) << oTurns << endl;

    //SECTION - Average number of turns per round
    int xAvg = round(xTurns / stats.turnsTaken.size());
    int oAvg = round(oTurns / stats.turnsTaken.size());
    cout << setprecision(0) << setw(colWidth) << "Average Turns" << setprecision(4) << setw(colWidth) << xAvg << setw(colWidth) << oAvg << endl;

    //SECTION - Avg Turns Per Win
    int xWinTurns = 0;
    for (int i:xIndices)
    {
        xWinTurns += xTurnsTaken[i];
    }
    if (xWins != 0)
        xWinTurns = xWinTurns / xWins;

    int oWinTurns = 0;
    for (int i:oIndices)
    {
        oWinTurns += oTurnsTaken[i];
    }
    if (oWins != 0)
        oWinTurns = oWinTurns / oWins;
    cout << setprecision(0) << setw(colWidth) << "Avg Turns Per Win" << setprecision(4) << setw(colWidth) << xWinTurns << setw(colWidth) << oWinTurns << endl;
    //endregion


    //SECTION - Board Info
    cout << setfill('~') << setw(3 * colWidth) << "~" << endl;
    cout << setfill(' ') << fixed;
    xPct = round((static_cast<double>(xAvg) / (stats.boardsize * stats.boardsize)) * 100);
    oPct = round((static_cast<double>(oAvg) / (stats.boardsize * stats.boardsize)) * 100);
    xPctString = std::to_string(xPct) + "%";
    oPctString = std::to_string(oPct) + "%";
    cout << setprecision(0) << setw(colWidth) << "BOARD INFORMATION:" << setprecision(4) << " " << setw(colWidth) << " " << setw(colWidth) << endl;
    cout << setprecision(0) << setw(colWidth) << "Board Size" << setprecision(4) << setw(colWidth) << stats.boardsize << setw(colWidth) << stats.boardsize << endl;
    cout << setprecision(0) << setw(colWidth) << "Total No. of Cells" << setprecision(4) << setw(colWidth) << stats.boardsize * stats.boardsize << setw(colWidth) << stats.boardsize * stats.boardsize
         << endl;
    cout << setprecision(0) << setw(colWidth) << "Avg % of Board Used" << setprecision(4) << setw(colWidth) << xPctString << setw(colWidth) << oPctString << endl;

    //SECTION - Round Duration
    cout << setfill('~') << setw(3 * colWidth) << "~" << endl;
    cout << setfill(' ') << fixed;
    cout << setprecision(0) << setw(colWidth) << "TIME INFORMATION:" << setprecision(4) << " " << setw(colWidth) << " " << setw(colWidth) << endl;
    int totalDur = 0.0;
    for (int i = 0; i < stats.durations.size(); i++)
    {
        totalDur += stats.durations[i];
        cout << setprecision(0) << setw(colWidth) << "Round " + to_string(i + 1) + " Duration" << setprecision(4) << setw(colWidth) << "(Seconds)" << setw(colWidth) << stats.durations[i]
             << endl;
    }
    totalDur = round(totalDur / stats.durations.size());
    cout << setprecision(0) << setw(colWidth) << "Avg Round Duration" << setprecision(4) << setw(colWidth) << "(Seconds)" << setw(colWidth) << totalDur << endl;
    //endregion

    cout << setfill('*') << setw(3 * colWidth) << "*" << endl;
    cout << setfill(' ') << setw(3 * colWidth) << " " << endl;
}
//y 1 1 6 3