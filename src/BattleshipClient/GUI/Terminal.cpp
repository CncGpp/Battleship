//
// Created by giuseppe on 27/12/16.
//

#include "Terminal.h"
void setCursorPos(int XPos, int YPos) {
    printf("\033[%d;%dH", YPos+1, XPos+1);
}


void printXY(int x, int y, string str){
    setCursorPos(x,y);
    cout << str;
    cout.flush();
}

void cprintXY(int x, int y, string color, string str){
    setCursorPos(x,y);
    string reset = Color::DEFAULT;
    cout << color << str << reset;
    cout.flush();
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}

std::string operator*(std::string const &s, size_t n)
{
    std::string r;  // empty string
    r.reserve(n * s.size());
    for (size_t i=0; i<n; i++)
        r += s;
    return r;
}

string csprint(string color, string str) {
    string reset = Color::color(Color::Reset,Color::Default,Color::Default);
    return color + str + reset;
}
