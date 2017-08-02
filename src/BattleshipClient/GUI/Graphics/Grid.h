//
// Created by giuseppe on 18/12/16.
//

#ifndef CLIENT_GRID_H
#define CLIENT_GRID_H
#include <iostream>
#include <vector>
#include "../Color.h"
#include "../Terminal.h"

#include "Drawable.h"

class Grid : public Drawable{
public:
    Grid(int x, int y, int size, int cellsize);

    virtual void draw();
    int getSize() const {return size;}
    int getWidth() const {return size+1+cellSize*size;}

protected:
    int size;
    int cellSize;
    bool labels;
    void _drawLabels();

    const string GRID_TOP = "┬";
    const string GRID_BOTTOM = "┴";
    const string GRID_NODE = "┼";
    const string GRID_VERTICAL = "│";
    const string GRID_HORIZONTAL = "─";
    const vector<string> GRID_EDGE = {"├","┤"};
    const vector<string> GRID_CORNER = {"┐","┌","└","┘"};
};


#endif //CLIENT_GRID_H
