//
// Created by giuseppe on 19/12/16.
//

#ifndef CLIENT_MARK_H
#define CLIENT_MARK_H
#include <iostream>
#include <vector>
#include "../Color.h"
#include "../Terminal.h"

#include "Drawable.h"

using namespace std;

class Mark : public Drawable{
public:
    enum MarkType{
        miss,
        hit,
        selected
    };

    Mark(int x, int y, MarkType type);
    Mark(MarkType type) : Mark(-1,-1,type){}
    virtual void draw();

    const MarkType &getType() const { return type; }
    void setType(const MarkType &type) { Mark::type = type; }

private:
    enum MarkType type;
    const vector<string> marks = {" ╳ ","▒▒▒","[ ]"};

};


#endif //CLIENT_MARK_H
