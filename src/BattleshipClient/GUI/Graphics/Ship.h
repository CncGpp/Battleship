//
// Created by giuseppe on 18/12/16.
//

#ifndef CLIENT_SHIP_H
#define CLIENT_SHIP_H
#include <iostream>
#include <vector>
#include "../Color.h"
#include "../Terminal.h"

#include "Drawable.h"

enum orientation{
    vertical,
    horizontal
};

class Ship :public Drawable{
public:
    Ship(int x, int y, int size, bool isHorizontal);
    Ship(int size, bool isHorizontal) : Ship(-1,-1,size,isHorizontal){}


    virtual void draw();
    bool intersect(Ship);

    int getSize() const { return size; }
    void setSize(int size) { Ship::size = size; }
    int getLenght();

    bool isHorizontal() const { return horizontal; }
    void rotate() {horizontal = !horizontal;}

    void setHorizontal(bool isHorizontal) { Ship::horizontal = isHorizontal; }

    bool hit() {if(++hits >= size) return true; else false;}
protected:
    int size;
    int hits;
    bool horizontal;
};


#endif //CLIENT_SHIP_H
