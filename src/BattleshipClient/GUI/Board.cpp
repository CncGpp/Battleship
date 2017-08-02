//
// Created by giuseppe on 19/12/16.
//

#include "Board.h"

void Board::draw() {

    grid.draw();
    for(auto x: ships) x.draw();
    for(auto x: marks) x.draw();

}

bool Board::addShip(int xCell, int yCell, Ship ship) {

    /*Verifico che i dati immessi siano validi*/
    if(xCell < 0 or xCell > grid.getSize()) return false;
    if(yCell < 0 or yCell > grid.getSize()) return false;

    //Verifico che la nave entri nei margini della griglia
    if(ship.isHorizontal() and xCell+ship.getSize() > grid.getSize()) return false;
    else if(!ship.isHorizontal() and yCell+ship.getSize() > grid.getSize()) return false;

    //Calcolo le coordinate relative alla xCell; yCell della griglia
    auto x = getXforCell(xCell);
    auto y = getYforCell(yCell);

    ship.setX(x);
    ship.setY(y);
    for(auto x : ships)
        if(x.intersect(ship)) return false;;

    ships.push_back(ship);
    return true;
}

bool Board::addMark(int xCell, int yCell, Mark mark) {
    /*Verifico che i dati immessi siano validi*/
    if(xCell < 0 or xCell > grid.getSize()) return false;
    if(yCell < 0 or yCell > grid.getSize()) return false;

    //Calcolo le coordinate relative alla xCell; yCell della griglia
    auto x = getXforCell(xCell);
    auto y = getYforCell(yCell);

    mark.setX(x);
    mark.setY(y);
    for(auto m : marks)
        if(m.getX() == mark.getX() and m.getY() == mark.getY()) return false;

    marks.push_back(mark);
    return true;
}




int Board::getXforCell(int xCell) {
    return grid.getX() + xCell + 3*xCell +1;
}

int Board::getYforCell(int yCell) {
    return grid.getY() + yCell  +yCell +1;
}

MoveReply::Result Board::hit(int xCell, int yCell) {
    /*Verifico che i dati immessi siano validi*/
    if(xCell < 0 or xCell > grid.getSize()) return MoveReply::miss;
    if(yCell < 0 or yCell > grid.getSize()) return MoveReply::miss;

    //Calcolo le coordinate relative alla xCell; yCell della griglia
    auto x = getXforCell(xCell);
    auto y = getYforCell(yCell);

    // Verifico che quel punto non sia stato già colpito...
    for(auto m : marks)
        if(m.getX() == x and m.getY() == y) return MoveReply::miss;

    //Vedo che ho colipito [Basta generare una nave 1x1 con coordinate pari a quelle del punto in cui colpisco]
    auto hitPoint = Ship(x,y,1,horizontal);
    for(auto& m : ships)
        if(m.intersect(hitPoint)) {
            addMark(xCell,yCell,Mark(Mark::hit));
            if ( m.hit() ) return MoveReply::hitAndSunk;
            return MoveReply::hit;
        }

    addMark(xCell,yCell,Mark(Mark::miss));
    return MoveReply::miss;
}

bool Board::isMarked(int xCell, int yCell) {
    /*Verifico che i dati immessi siano validi*/
    if(xCell < 0 or xCell > grid.getSize()) return false;
    if(yCell < 0 or yCell > grid.getSize()) return false;

    //Calcolo le coordinate relative alla xCell; yCell della griglia
    auto x = getXforCell(xCell);
    auto y = getYforCell(yCell);

    for(auto m : marks)
        if(m.getX() == x and m.getY() == y) return true;

    return false;
}
