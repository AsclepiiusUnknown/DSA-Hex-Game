#ifndef ASTAR_H
#define ASTAR_H

class AStar
{
public:
    stack <ASCell> EvaluateAStar(Board board, int player, Cell _src)
    {
//        printf("\nEvaluateAStar Start\n");
        ASCell src(_src.x, _src.y);
        if (player != -1 && player != 1)
        {
            printf("\nERROR: No player recognised by AStar's Best Move.\n");
            return {};
        }
//        printf("\nEvaluateAStar End\n");

        return EvaluateCell(board, player, src);
    }

    int CheckPreowned(Board board, int player, ASCell src, bool &start, bool &finish)
    {
//        printf("\nCheckPreowned Start\n");

        int leftTop = 0, rightBottom = board.GetBoardSize() - 1;

        stack <Cell> preOpen = board.CheckNeighbours(player, src.x, src.y);
        vector <Cell> preClosed;
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
                stack <Cell> n = board.CheckNeighbours(player, c.x, c.y); //Get empty neighbours
                while (!n.empty())
                {
                    if (!board.isInVector(preClosed, n.top()))
                        preOpen.push(n.top());
                    n.pop();
                }
            }
        }
//        printf("\nCheckPreowned End\n");
        return 0;
    }

    stack <ASCell> EvaluateCell(Board board, int player, ASCell src)
    {
//        printf("\nEvaluateCell Start\n");

        int bs = board.GetBoardSize();
        bool start = false, finish = false;
        int leftTop = 0, rightBottom = bs - 1;

        if (CheckPreowned(board, player, src, start, finish) == 1)
        {
            stack <ASCell> pre;
            pre.push(ASCell(src.x, src.y));
            return pre;
        }

        priority_queue <ASCell, vector<ASCell>, greater<ASCell>> open;
        vector <ASCell> closed;
        stack <ASCell> tempN = CheckNeighbours(board, 0, src.x, src.y);
//        cout << "Neighbours SIZE: " << tempN.size() << endl;
        while (!tempN.empty())
        {
            open.push(ASCell(tempN.top().x, tempN.top().y));
            tempN.pop();
        }
//        cout << "Open SIZE: " << open.size() << endl;

        if (open.empty())
        {
            printf("\nEvaluateCell End. Open Stack was empty.\n");
            return {};
        }


        while (!open.empty())
        {
//            printf("\nMARK 1\n");
            //c = best open cell
            ASCell c = open.top();
            open.pop();
//            cout << "New Open SIZE: " << open.size() << endl;
            closed.push_back(c);

//            printf("\nMARK 2\n");

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
            printf("\nMARK 3\n");

            if (start && finish)
            {
                ReconstructPath(bs, src, c);
            }
            else
            {
                stack <ASCell> n = CheckNeighbours(board, player, c.x, c.y); //Get empty neighbours
                cout << "Current's Neighbours SIZE: " << open.size() << endl;
                while (!n.empty())
                {
                    printf("\nMARK 4\n");
                    if (!isInVector(closed, ASCell(n.top().x, n.top().y)))
                    {
                        n.top().SetParent(&c);
                        double g, h;
                        g = c.G() + 1.0;
                        h = 0.0;
                        switch (player)
                        {
                            case -1:
                                if (!start)
                                    h += abs(n.top().y);
                                if (!finish)
                                    h += abs(bs - n.top().y);
                                break;
                            case 1:
                                if (!start)
                                    h += n.top().x;
                                if (!finish)
                                    h += bs - n.top().x;
                                break;
                        }
                        n.top().SetValues(g, h);

                        open.push(n.top());
                    }
                    n.pop();
                }
            }
        }
        printf("\nEvaluateCell End\n");

        return {};
    }

    stack <ASCell> ReconstructPath(int failSafe, ASCell src, ASCell current)
    {
        stack <ASCell> path;
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

    bool isInVector(vector <ASCell> s, ASCell e)
    {
        if (s.empty())
            return false;

        for (ASCell i : s)
            if (i.x == e.x && i.y == e.y)
                return true;

        return false;
    }

    stack <ASCell> CheckNeighbours(Board board, int target, int x, int y);
};


stack <ASCell> AStar::CheckNeighbours(Board board, int target, int x, int y)
{
    stack <ASCell> neighbours;
    int **grid = board.GetGrid();
    int bs = board.GetBoardSize();

    //Left
    if ((y - 1) >= 0)
    {
        if (grid[x][y - 1] == target || grid[x][y - 1] == 0)
        {
            neighbours.push(ASCell(x, y - 1));
        }
    }

    //Right
    if ((y + 1) < bs)
    {
        if (grid[x][y + 1] == target || grid[x][y + 1] == 0)
        {
            neighbours.push(ASCell(x, y + 1));
        }
    }

    //Up-Left
    if ((x - 1) >= 0)
    {
        if (grid[x - 1][y] == target || grid[x - 1][y] == 0)
        {
            neighbours.push(ASCell(x - 1, y));
        }
    }

    //Up-Right
    if ((x - 1) >= 0 && (y + 1) < bs)
    {
        if (grid[x - 1][y + 1] == target || grid[x - 1][y + 1] == 0)
        {
            neighbours.push(ASCell(x - 1, y + 1));
        }
    }

    //Down-Left
    if ((x + 1) < bs && (y - 1) >= 0)
    {
        if (grid[x + 1][y - 1] == target || grid[x + 1][y - 1] == 0)
        {
            neighbours.push(ASCell(x + 1, y - 1));
        }
    }

    //Down-Right
    if ((x + 1) < bs)
    {
        if (grid[x + 1][y] == target || grid[x + 1][y] == 0)
        {
            neighbours.push(ASCell(x + 1, y));
        }
    }

    return neighbours;
}

#endif //ASTAR_H
