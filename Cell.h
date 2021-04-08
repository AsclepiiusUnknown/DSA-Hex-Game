/*
 * Cell.h
 *
 *  Created on: 26 Mar. 2019
 *      Author: dongmo
 */

#ifndef CELL_H_
#define CELL_H_

struct Cell
{
    int x;
    int y;
    double heuristic;

    Cell(int xx, int yy, double hh) : x(xx), y(yy), heuristic(hh)
    {}

    bool operator<(const Cell &c) const
    {
        return heuristic < c.heuristic;
    }
};

#endif /* CELL_H_ */
