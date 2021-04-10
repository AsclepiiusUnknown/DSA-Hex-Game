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

#endif /* CELL_H_ */
