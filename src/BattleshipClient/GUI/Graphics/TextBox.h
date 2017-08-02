//
// Created by giuseppe on 22/12/16.
//

#ifndef CLIENT_TEXTBOX_H
#define CLIENT_TEXTBOX_H

#include <iostream>
#include <list>
#include "Box.h"

using namespace std;

class TextBox : public Box{
public:
    TextBox(int x, int y, int width, int height) : Box(x,y,width,height){
        this->filled = true;
    }
    void draw();
    void push(string str);

private:
    list<string> textRows;
};


#endif //CLIENT_TEXTBOX_H
