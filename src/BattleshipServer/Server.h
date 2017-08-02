//
// Created by giuseppe on 26/12/16.
//

#ifndef BATTLESHIPSERVER_SERVER_H
#define BATTLESHIPSERVER_SERVER_H

#include <iostream>
#include <thread>
#include <map>
#include "lib.h"
#include "Wrapper.h"
#include "Package.h"

using namespace std;

class Server {
public:
    Server(short port) : port(port){}
    virtual ~Server() { close(listenfd); close(connfd); }
    virtual int start() = 0;

protected:
    short port;
    int listenfd, connfd;
    struct sockaddr_in  servaddr;
    int waitRequest(int connfd, Package::Type* type);
    int sendReply(int connfd, Package::Status status);
    int sendPkg(int connfd, Package* pkg);
    template<typename T> int waitPkg(int connfd, T* pkg);

};


#endif //BATTLESHIPSERVER_SERVER_H
