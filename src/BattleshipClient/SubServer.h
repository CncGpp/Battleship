//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPCLIENT_SUBSERVER_H
#define BATTLESHIPCLIENT_SUBSERVER_H
#include <iostream>
#include <Package.h>
#include "lib.h"

using namespace std;
class SubServer {
public:
    SubServer(){}
    virtual ~SubServer() { close(listenfd); close(connfd); }
    int runSubServer();
    int listenForInvite(string &foeUsername);
    int sendInviteReply(bool);

    int waitReady();
    int sendReady();

    int sendMove(int x, int y);
    int waitForMove(MoveRequest& m);
    int sendMoveReply(MoveReply::Result);

    short getPort() const { return port; }

protected:
    short port;
    int listenfd, connfd;
    struct sockaddr_in servaddr;

    int sendRequest(int connfd, Package::Type type);
    int waitRequest(int connfd, Package::Type* type);
    int sendReply(int connfd, Package::Status status);
    int sendPkg(int connfd, Package* pkg);
    template<typename T> int waitPkg(int connfd, T* pkg);
};



#endif //BATTLESHIPCLIENT_SUBSERVER_H
