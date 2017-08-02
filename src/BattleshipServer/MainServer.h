//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPSERVER_BATTLESHIPSERVER_H
#define BATTLESHIPSERVER_BATTLESHIPSERVER_H


#include <Package.h>
#include "Server.h"
#include "data/RegistredUsers.h"
#include "UsersList.h"
#include "data/MatchList.h"

class MainServer : public Server{
public:
    MainServer(short port = 1024) : Server(port){}
    int start();

protected:
    void manageClient(int _connfd, struct sockaddr_in  clientaddr);
    string loginReply(int _connfd, struct sockaddr_in clientaddr);
    int onlineListReply(int _connfd);
    int freeListReply(int _connfd);
    int challangeReply(int _connfd);
    int matchReply(int _connfd, const string &connectedUsername, string &foeUsername);

    int sendTerminatedMatches(int _connfd, string connectedUsername);
    int sendInProgressMatches(int _connfd, string connectedUsername);
    int sendRegistredUsers(int _connfd, string connectedUsername);

private:
    RegistredUsers registredList;
    UsersList onlineList;
    UsersList freeList;
    MatchList matches;
};


#endif //BATTLESHIPSERVER_BATTLESHIPSERVER_H
