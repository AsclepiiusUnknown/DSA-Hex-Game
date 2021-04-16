#ifndef CELL_H_
#define CELL_H_

struct Cell
{
    int x, y;

    Cell(int _x, int _y) : x(_x), y(_y)
    {}
};

struct Move
{
    int x, y;
    double v;

    Move(int _x, int _y, double _v) : x(_x), y(_y), v(_v)
    {}

    bool operator<(const Move &m) const
    {
        return v < m.v;
    };
};

class PathCell
{
public:
    int x, y;
    int length = 0, h = 0;
    PathCell *parent = NULL;
    vector <PathCell> visited;

    double GetValue()
    { return (h - (length / 3)); }

    bool Compare(PathCell other)
    {
        if (x == other.x && y == other.y && parent == other.parent && GetValue() == other.GetValue())
            return true;

        return false;
    }

    PathCell(int _x, int _y, PathCell *_parent) : x(_x), y(_y), parent(_parent)
    {}

    PathCell(int _x, int _y, PathCell *_p, int _h, int _l, vector <PathCell> _v) : x(_x), y(_y), parent(_p), h(_h), length(_l), visited(_v)
    {}
};

class ASCell //For use in AStar
{
private:
    double f, g, h;
    ASCell *parent;
public:
    int x, y;

    void SetValues(double _h, double _g)
    {
        h = _h;
        g = _g;
        f = g + h;
    }

    double G() const
    { return g; }

    double F() const
    { return f; }

    ASCell Parent() const
    { return *parent; }

    void SetParent(ASCell *_p)
    {
        parent = _p;
    }

    ASCell(int _x, int _y) : x(_x), y(_y)
    {
        g = 1000;
        h = 1000;
        f = g + h;
        parent = NULL;
    }

    bool operator>(const ASCell &asCell) const
    {
        return F() > asCell.F();
    };
};


#endif /* CELL_H_ */
