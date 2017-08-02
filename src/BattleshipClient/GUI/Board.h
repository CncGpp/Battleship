//
// Created by giuseppe on 19/12/16.
//

#ifndef CLIENT_BOARD_H
#define CLIENT_BOARD_H

#include <iostream>
#include <vector>
#include <Package.h>
#include "Graphics/Grid.h"
#include "Graphics/Ship.h"
#include "Graphics/Mark.h"

using namespace std;

class Board {

public:
    Board(int x, int y, int size) : grid(x,y,size,3){ }

    Grid grid;
    void draw();
    bool addShip(int xCell, int yCell, Ship ship);
    bool addMark(int xCell, int yCell, Mark ship);
    MoveReply::Result hit(int xCell, int yCell);
    bool isMarked(int xCell, int yCell);
    int getXforCell(int xCell);
    int getYforCell(int yCell);

private:
    vector<Ship> ships;
    vector<Mark> marks;
};


#endif //CLIENT_BOARD_H
