//
// Created by giuseppe on 18/12/16.
//

#include "Grid.h"
#include "Box.h"


Grid::Grid(int x, int y, int size, int cellsize) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->cellSize = cellsize;
    this->labels = true;
}


void Grid::draw() {
    //Creo il colore da utilizzare per la stampa
    const string mycolor = Color::color(attrColor,foreColor,backColor);
    //Riempio tutta la griglia
    {

        auto b = Box(x, y, size+1+cellSize*size, size+1+size);
        b.setFilled(true);
        b.draw();
    }
    //Genero il lato della singola cella;
    string CELL_HORIZONTAL = GRID_HORIZONTAL * cellSize;

    //Genero il lato superiore
    string TOP = CELL_HORIZONTAL;
    TOP+=GRID_TOP;
    TOP = TOP * (size - 1) ;
    TOP += CELL_HORIZONTAL;
    cprintXY(x,y,mycolor,GRID_CORNER[1] + TOP + GRID_CORNER[0]);

    int k = 0;
    for(; k < size*2; k++)
    for(int i = 0; i < size + 1; i++){
        cprintXY(x + i*(cellSize+1),y+k+1,mycolor,GRID_VERTICAL);
    }

    //Genero le line intermedie;
    string MIDDLE = CELL_HORIZONTAL;
    MIDDLE+=GRID_NODE;
    MIDDLE = MIDDLE * (size -1 );
    MIDDLE+= CELL_HORIZONTAL;
    for(int i = 0; i < size; i++)
    cprintXY(x,y+2 + i*(2),mycolor,GRID_EDGE[0] + MIDDLE + GRID_EDGE[1]);

    //Genero il lato inferiore
    string BOTTOM = CELL_HORIZONTAL;
    BOTTOM+=GRID_BOTTOM;
    BOTTOM = BOTTOM * (size - 1) ;
    BOTTOM += CELL_HORIZONTAL;
    cprintXY(x,y + k,mycolor,GRID_CORNER[2] + BOTTOM + GRID_CORNER[3]);

    _drawLabels();
    setCursorPos(0,0);
    cout << endl;
}

void Grid::_drawLabels() {
    if(!labels) return;

    for(int i = 0; i < size; i++)
        printXY(x + cellSize/2 + 1 + (cellSize+1)*(i),y-1,std::to_string(i));

    for(int i = 0; i < size; i++){
        char a = 'A';
        printXY(x - 2,y+1 +2*i,string(1,a + i));

    }
}
