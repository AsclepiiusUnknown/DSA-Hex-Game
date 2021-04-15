#ifndef ASTAR_H_
#define ASTAR_H_

#include "../AStar.h"

class AStarPlayer : public Player
{
public:
    AStarPlayer(int t, string symbol = "Undefined (ERROR)", string name = "A*") :
            Player(t, symbol, name)
    {
    }

    const int MAX = 1000;
    const int MIN = -1000;
    const int WIN_VAL = 10;
    AStar aStar;

    bool GetMove(Board *board, int &x, int &y);


public:
    stack<ASCell> EvaluateAStar(Board board, int player, Cell _src)
    {
        printf("\nEvaluateAStar Start\n");
        ASCell src(_src.x, _src.y);
        if (player != -1 && player != 1)
        {
            printf("\nERROR: No player recognised by AStar's Best Move.\n");
            return {};
        }
        printf("\nEvaluateAStar End\n");

        return EvaluateCell(board, player, src);
    }

    int CheckPreowned(Board board, int player, ASCell src, bool &start, bool &finish)
    {
        printf("\nCheckPreowned Start\n");

        int leftTop = 0, rightBottom = board.getBoardSize() - 1;

        stack<Cell> preOpen = board.CheckNeighbours(player, src.x, src.y);
        vector<Cell> preClosed;
        while (!preOpen.empty())
        {
            Cell c = preOpen.top();
            preOpen.pop();
            preClosed.push_back(c);

            switch (player)
            {
                case -1:
                    start = (start) ? start : (c.y == leftTop);
                    finish = (finish) ? finish : (c.y == rightBottom);
                    break;
                case 1:
                    start = (start) ? start : (c.x == leftTop);
                    finish = (finish) ? finish : (c.x == rightBottom);
                    break;
            }

            if (start && finish)
            {
                printf("\nERROR: AStar found win condition was already met.\n");
                return 1;
            }
            else
            {
                stack<Cell> n = board.CheckNeighbours(player, c.x, c.y); //Get empty neighbours
                while (!n.empty())
                {
                    if (!board.isInVector(preClosed, n.top()))
                        preOpen.push(n.top());
                    n.pop();
                }
            }
        }
        printf("\nCheckPreowned End\n");
        return 0;
    }

    stack<ASCell> EvaluateCell(Board board, int player, ASCell src)
    {
        printf("\nEvaluateCell Start\n");

        int bs = board.getBoardSize();
        bool start = false, finish = false;
        int leftTop = 0, rightBottom = bs - 1;

        if (CheckPreowned(board, player, src, start, finish) == 1)
        {
            stack<ASCell> pre;
            pre.push(ASCell(src.x, src.y));
            return pre;
        }

        priority_queue<ASCell, vector<ASCell>, greater<ASCell>> open;
        vector<ASCell> closed;
        stack<Cell> tempN = board.CheckNeighbours(0, src.x, src.y);
        while (!tempN.empty())
        {
            open.push(ASCell(tempN.top().x, tempN.top().y));
            tempN.pop();
        }

        if (open.empty())
        {
            printf("\nEvaluateCell End. Open Stack was empty.\n");
            return {};
        }


        while (!open.empty())
        {
            //c = best open cell
            ASCell c = open.top();
            open.pop();
            closed.push_back(c);

            //if c == goal
            switch (player)
            {
                case -1:
                    start = (start) ? start : (c.y == leftTop);
                    finish = (finish) ? finish : (c.y == rightBottom);
                    break;
                case 1:
                    start = (start) ? start : (c.x == leftTop);
                    finish = (finish) ? finish : (c.x == rightBottom);
                    break;
            }

            if (start && finish)
            {
                ReconstructPath(bs, src, c);
            }
            else
            {
                stack<Cell> n = board.CheckNeighbours(0, c.x, c.y); //Get empty neighbours
                while (!n.empty())
                {
                    if (!isInVector(closed, ASCell(n.top().x, n.top().y)))
                    {
                        ASCell thisN(n.top().x, n.top().y);
                        thisN.SetParent(&c);
                        double g, h;
                        g = c.G() + 1.0;
                        h = 0.0;
                        switch (player)
                        {
                            case -1:
                                if (!start)
                                    h += abs(thisN.y);
                                if (!finish)
                                    h += abs(bs - thisN.y);
                                break;
                            case 1:
                                if (!start)
                                    h += thisN.x;
                                if (!finish)
                                    h += bs - thisN.x;
                                break;
                        }
                        thisN.SetValues(g, h);

                        open.push(ASCell(n.top().x, n.top().y));
                    }
                    n.pop();
                }
            }
        }
        printf("\nEvaluateCell End\n");

        return {};
    }

    stack<ASCell> ReconstructPath(int failSafe, ASCell src, ASCell current)
    {
        stack<ASCell> path;
        ASCell next = current;
        while (failSafe > 0)
        {
            path.push(next);
            next = next.Parent();

            if (path.top().Parent().x == src.x || path.top().Parent().y == src.y)
                failSafe--;
        }
        if (path.empty())
            printf("\nERROR: Reconstruction of AStar Path resulted in a path with no elements.\n");
        return path;
    }

    bool isInVector(vector<ASCell> s, ASCell e)
    {
        if (s.empty())
            return false;

        for (ASCell i : s)
            if (i.x == e.x && i.y == e.y)
                return true;

        return false;
    }
};

bool AStarPlayer::GetMove(Board *board, int &x, int &y)
{
    int bs = board->getBoardSize();

    for (int r = 0; r < bs; r++)
        for (int c = 0; c < bs; c++)
            EvaluateAStar(*board, type, Cell(r, c));

    return true;
}


#endif /* ASTAR_H_ */
