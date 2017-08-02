//
// Created by giuseppe on 18/12/16.
//

#include "Box.h"



Box::Box(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->attrColor = Color::Reset;
    this->foreColor = Color::Default;
    this->backColor = Color::Default;
    this->title = "";
    this->filled = false;
}


void Box::_drawTitle() {
    if(title.size() == 0 or title.size() > width-2) return;

    printXY(x+width/2-title.size()/2 -1 ,y," " + title + " ");
}

void Box::draw() {
    //Creo il colore da utilizzare per la stampa della box
    const string mycolor = Color::color(attrColor,foreColor,backColor);

    //Genero e stampo il lato superiore
    string TOP = BOX_TOP*(width-2);
    cprintXY(x,y,mycolor,BOX_CORNER[1] + TOP + BOX_CORNER[0]);

    //Genero e stampo il lato inferiore
    string BOTTOM = BOX_BOTTOM*(width-2);
    cprintXY(x,y+height-1,mycolor,BOX_CORNER[2] + BOTTOM + BOX_CORNER[3]);

    //Genero e stampo i lati
    for(int i = 1; i<height-1; i++){
        if(isFilled()) cprintXY(x,y+i,mycolor, string(" ")*width);
        cprintXY(x,y+i,mycolor,BOX_EDGE);
        cprintXY(x+width-1,y+i,mycolor,BOX_EDGE);
    }


    /* Aggiungo eventualmente un titolo alla box */
    _drawTitle();
    setCursorPos(0,0);
    cout << endl;

}