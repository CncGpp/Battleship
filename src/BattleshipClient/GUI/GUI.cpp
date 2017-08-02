//
// Created by giuseppe on 27/12/16.
//

#include <chrono>
#include "GUI.h"
#include "Terminal.h"
#include "Graphics/Box.h"

void GUI::initScreenSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    lines = w.ws_row;
    column = w.ws_col;
}


void GUI::_drawTitle() {
    int x, y;
    auto color = Color::DEFAULT;

    auto titleLen = 76;
    y = 4;
    x = column/2 - titleLen/2;

    cprintXY(x,y++,color,"██████╗  █████╗ ████████╗████████╗██╗     ███████╗███████╗██╗  ██╗██╗██████╗");
    cprintXY(x,y++,color,"██╔══██╗██╔══██╗╚══██╔══╝╚══██╔══╝██║     ██╔════╝██╔════╝██║  ██║██║██╔══██╗");
    cprintXY(x,y++,color,"██████╔╝███████║   ██║      ██║   ██║     █████╗  ███████╗███████║██║██████╔╝");
    cprintXY(x,y++,color,"██╔══██╗██╔══██║   ██║      ██║   ██║     ██╔══╝  ╚════██║██╔══██║██║██╔═══╝");
    cprintXY(x,y++,color,"██████╔╝██║  ██║   ██║      ██║   ███████╗███████╗███████║██║  ██║██║██║");
    cprintXY(x,y++,color,"╚═════╝ ╚═╝  ╚═╝   ╚═╝      ╚═╝   ╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝╚═╝");

    string io = "Giuseppe Cianci - 0124001064";
    color = Color::color(Color::Bright, Color::Default, Color::Default);
    cprintXY(column/2 - (int)io.size()/2,y++,color,io);
}


void GUI::loginScene(string &username, string &password) {
    _terminalSizeCheck();
    clearScreen();
    initScreenSize();

    auto lines = this->lines;
    auto column = this->column;
    int boxLines = 24;

    _drawTitle();

    auto outline = Box(0,0,column,lines);
    outline.setAttrColor(Color::Dim);

    auto loginBox = Box(column/2-boxLines/2,lines/2,boxLines,3);
    loginBox.setTitle("Username");

    auto passwordBox = Box(column/2-boxLines/2,lines/2 + 4,boxLines,3);
    passwordBox.setTitle("Password");

    outline.draw();
    loginBox.draw();
    passwordBox.draw();

    setCursorPos(loginBox.getX()+1,loginBox.getY()+1);
    cin >> username;

    setCursorPos(passwordBox.getX()+1,passwordBox.getY()+1);
    cin >> password;

}
void GUI::loginErrorScene(){
    auto myColor = Color::color(Color::Bright, Color::Red, Color::Default);
    auto lines = this->lines;
    auto column = this->column;

    auto warning = Box(column/2-column/4-3,lines/2-2,column/2+7,4);
    warning.setFilled(true);
    warning.setAttrColor(Color::Bright);
    warning.setForeColor(Color::Red);
    warning.draw();

    string message = "Credenziali errate oppure già connesso";
    cprintXY(warning.getX() + warning.getWidth()/2 - (int)message.size()/2 ,
             warning.getY() + warning.getHeight()/2-1,
             myColor,
             message
    );
    message = "Premi un tasto per riprovare...";
    cprintXY(warning.getX() + warning.getWidth()/2 - (int)message.size()/2 ,
             warning.getY() + warning.getHeight()/2,
             myColor,
             message
    );

    getchar();
    getchar();
}


void GUI::messageBox(string message, short color){
    auto mycolor = Color::color(Color::Bright, color, Color::Default );
    auto lines = this->lines;
    auto column = this->column;

    int length = (int)message.size();

    auto mybox = Box(column / 2 - length / 2 - 2, lines / 2 - 3, length + 6 , 3);
    mybox.setFilled(true);
    mybox.setAttrColor(Color::Bright);
    mybox.setForeColor(color);
    mybox.draw();

    cprintXY(mybox.getX() + mybox.getWidth() / 2 - length / 2, mybox.getY() + 1, mycolor, message);
}


void GUI::_terminalSizeCheck() {
    clearScreen();
    initScreenSize();
    string m1 = "Per favore, aumentare la dimensione della finestra!";
    string m2 = "Numero di righe minimo: 30    \t Attuale: ";
    string m3 = "Numero di colonne minimo: 100 \t Attuale: ";


    auto outline = Box(0,0,column,lines);
    outline.setForeColor(Color::White);
    outline.setAttrColor(Color::Dim);
    outline.setFilled(true);

    while(lines < 30 or column < 100){
        initScreenSize();

        outline.setHeight(lines);
        outline.setWidth(column);
        outline.draw();

        cprintXY(column/2 - (int)m1.size()/2, lines/3,Color::WHITE,m1);

        cprintXY(column/2 - (int)m2.size()/2 -2 , lines/2,Color::WHITE,m2 + to_string(lines));
        cprintXY(column/2 - (int)m3.size()/2 -2 , lines/2+1,Color::WHITE,m3 + to_string(column));

        sleep(1);
    }
}
