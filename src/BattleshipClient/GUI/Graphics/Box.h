//
// Created by giuseppe on 18/12/16.
//

#ifndef CLIENT_BOX_H
#define CLIENT_BOX_H

#include <iostream>
#include <vector>
#include "../Color.h"
#include "../Terminal.h"

#include "Drawable.h"

using namespace std;

class Box : public Drawable {
public:
    virtual void draw();

    Box(int x, int y, int width, int height);

    const string &getTitle() const { return title; }
    void setTitle(const string &title) { Box::title = title; }

    int getHeight() const { return height; }
    void setHeight(int height) { Box::height = height; }
    int getWidth() const { return width; }
    void setWidth(int width) { Box::width = width; }
    bool isFilled() const { return filled; }
    void setFilled(bool filled) { Box::filled = filled; }

protected:
    int width, height;

    bool filled;
    string title;
    void _drawTitle();

    const string BOX_TOP = "─";
    const string BOX_BOTTOM = "─";
    const string BOX_EDGE = "│";
    const vector<string> BOX_CORNER = {"┐","┌","└","┘"};

};


#endif //CLIENT_BOX_H
