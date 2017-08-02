//
// Created by giuseppe on 22/12/16.
//

#include "TextBox.h"

void TextBox::draw() {
    Box::draw();
    int _x = this->getX() + 1;
    int _y = this->getY() + 1;

    int i = 0;
    for(auto row : textRows){
        printXY(_x,_y + i ,row + '\0');
        cout << endl;
        i++;
    }
}

void TextBox::push(string str) {
    textRows.push_front(str);

    if(textRows.size() > Box::getHeight() - 2) textRows.pop_back();
}
