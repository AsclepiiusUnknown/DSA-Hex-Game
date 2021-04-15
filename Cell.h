#ifndef CELL_H_
#define CELL_H_

struct Cell
{
    int x, y; //coordinates for the cell

    Cell(int _x, int _y) : x(_x), y(_y)
    {}

};

struct Move
{
    int x, y; //coordinates for the move
    double v; //Cell value

    Move(int _x, int _y, double _v) : x(_x), y(_y), v(_v)
    {}

    bool operator<(const Move &m) const
    {
        return v < m.v;
    };
};

class ASCell //For use in AStar
{
private:
    double f, g, h;
    ASCell *parent;
public:
    int x, y; //coordinates for the move

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
