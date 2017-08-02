//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPCLIENT_COLOR_H
#define BATTLESHIPCLIENT_COLOR_H
#include <iostream>
using namespace std;

#define CONSOLE_COLOR_DEFAULT "\033[0m"
#define FORECOLOR(C) "\033[" #C "m"
#define BACKCOLOR(C) "\033[" #C "m"
#define ATTR(A) "\033[" #A "m"

class Color {
public:
    enum
    {
        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        Default = 9
    };

    enum Attributes
    {
        Reset,
        Bright,
        Dim,
        Underline,
        Blink,
        Reverse,
        Hidden
    };


    static string color(int attr, int fg, int bg)
    {
        static char command[13];
        /* Command is the control command to the terminal */
        sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
        return command;
    }

    static const string WHITE;
    static const string DEFAULT;
};
#endif //BATTLESHIPCLIENT_COLOR_H
