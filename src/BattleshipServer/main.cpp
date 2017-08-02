#include <iostream>
#include <stdlib.h>
#include "MainServer.h"

using namespace std;

int main(int argc, char* argv[]) {

    short port = 1024;

    int c = 0;
    while((c = getopt(argc, argv, "p:")) != -1){
        switch(c){
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-p] [port]\n", argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    MainServer s(port);
    s.start();
    return 0;
}