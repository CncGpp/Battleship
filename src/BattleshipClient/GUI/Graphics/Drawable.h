//
// Created by giuseppe on 18/12/16.
//

#ifndef CLIENT_DRAWABLE_H
#define CLIENT_DRAWABLE_H


class Drawable{
public:
    Drawable();
    virtual void draw() = 0;

    short getAttrColor() const { return attrColor; }
    void setAttrColor(short attrColor) { this->attrColor = attrColor; }
    short getForeColor() const { return foreColor; }
    void setForeColor(short foreColor) { this->foreColor = foreColor; }
    short getBackColor() const { return backColor; }
    void setBackColor(short backColor) { this->backColor = backColor; }
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int x) { Drawable::x = x; }
    void setY(int y) { Drawable::y = y; }

protected:
    int x, y;

    short attrColor;
    short foreColor;
    short backColor;
};



#endif //CLIENT_DRAWABLE_H
