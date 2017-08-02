//
// Created by giuseppe on 18/12/16.
//

#include "Ship.h"
#include "Box.h"

Ship::Ship(int x, int y, int size, bool isHorizontal) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->horizontal = isHorizontal;

    this->hits = 0;
}

void Ship::draw() {
    if(size == 1){
        auto color = Color::color(getAttrColor(),getForeColor(),getBackColor());
        cprintXY(x,y,color," ⎕ ");
        return;
    }
    int width = horizontal ? 1 : 3;
    int heigth = horizontal ? size*3 + (size-1): size*2 - 1;

    if(isHorizontal()) swap(width,heigth);
    auto b = Box(x ,y ,width,heigth);
    b.setForeColor(this->getForeColor());
    b.setBackColor(this->getBackColor());
    b.setAttrColor(this->getAttrColor());
    b.setFilled(true);
    b.draw();
}



int Ship::getLenght() {
    if(isHorizontal()) return size*3;
    else return size*2-1;
}


bool Ship::intersect(Ship ship) {

    auto& s1 = *this;
    auto& s2 = ship;



    if(s1.isHorizontal() and s2.isHorizontal()){
        if(s1.getY() != s2.getY()) return false;

        if(s1.getX() < s2.getX())
            return s1.getX()+s1.getLenght()>= s2.getX();
        else
            return s2.getX()+s2.getLenght()>= s1.getX();
    }

    if(!s1.isHorizontal() and !s2.isHorizontal()){
        if(s1.getX() != s2.getX()) return false;

        if(s1.getY() < s2.getY())
            return s1.getY()+s1.getLenght()>= s2.getY();
        else
            return s2.getY()+s2.getLenght()>= s1.getY();
    }

    int _x, _y;
    if(s1.isHorizontal() and !s2.isHorizontal()) {
        _x = s2.getX(); _y = s1.getY();
        if(_x >= s1.getX() and _x <= s1.getX() + s1.getLenght())
        if(_y >= s2.getY() and _y <= s2.getY() + s2.getLenght()) return true;
    } else{
        _x = s1.getX(); _y = s2.getY();
        if(_y >= s1.getY() and _y <= s1.getY() + s1.getLenght())
        if(_x >= s2.getX() and _x <= s2.getX() + s2.getLenght()) return true;
    }

    return false;
}
