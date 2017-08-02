//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPCLIENT_TERMINAL_H
#define BATTLESHIPCLIENT_TERMINAL_H
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "Color.h"
using namespace std;
std::string operator*(std::string const &s, size_t n);
void setCursorPos(int XPos, int YPos);
void printXY(int x, int y, string str);
void cprintXY(int x, int y, string color, string str);
string csprint(string color, string str);

char getch();

#endif //BATTLESHIPCLIENT_TERMINAL_H
