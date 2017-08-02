//
// Created by giuseppe on 19/12/16.
//

#include "Mark.h"

void Mark::draw() {
    //Creo il colore da utilizzare per la stampa della box
    const string mycolor = Color::color(attrColor,foreColor,backColor);

    if(type == selected){
        string s1(1, marks[selected][0]);
        string s2(1, marks[selected][2]);
        cprintXY(x,y,mycolor,s1);
        cprintXY(x+2,y,mycolor,s2);
        return;
    }
    cprintXY(x,y,mycolor,marks[type]);
}

Mark::Mark(int x, int y, Mark::MarkType type) {
    this->x = x;
    this->y = y;
    this->type = type;
}
