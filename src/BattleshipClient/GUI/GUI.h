//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPCLIENT_GUI_H
#define BATTLESHIPCLIENT_GUI_H
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
using namespace std;

enum {
    UP_ARROW = 65,
    DOWM_ARROW,
    RIGHT_ARROW,
    LEFT_ARROW,
    ENTER = 10
};


class GUI{
public:
    unsigned int lines;   // Y
    unsigned int column;  // X

    GUI(){ initScreenSize(); }
    virtual ~GUI() { }

    void clearScreen(){ std::system("clear"); }
    void initScreenSize();


    /*FUNZIONI*/
    void loginScene(string& username, string& password);
    void loginErrorScene();
    void messageBox(string message, short color);
protected:
    void _drawTitle();
    void _terminalSizeCheck();
};




#endif //BATTLESHIPCLIENT_GUI_H
