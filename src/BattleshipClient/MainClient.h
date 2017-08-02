//
// Created by giuseppe on 26/12/16.
//

#ifndef BATTLESHIPCLIENT_MAINCLIENT_H
#define BATTLESHIPCLIENT_MAINCLIENT_H
#include "Client.h"
#include "Package.h"
#include "UsersList.h"


class MainClient : public Client{
public:
    MainClient(){}
    virtual ~MainClient() { }

    int loginRequest(std::string username, std::string password, short port);
    int onlineListRequest();
    int freeListRequest();
    int challangerRequest(string username, struct sockaddr_in *servaddr);
    int sendNotify(Package::Type notify);
    int matchRequest(string foeUsername);

    bool isLogged() const { return logged; }
    bool isAdmin() const { return admin; }

    int terminatedMatchesRequest();
    int inProgressMatchesRequest();
    int registeredUsersRequest();

    UsersList &getOnlineList() { return onlineList; }
    UsersList &getFreeList() { return freeList; }

private:
    bool logged;
    bool admin;

    UsersList onlineList;
    UsersList freeList;
};


#endif //BATTLESHIPCLIENT_MAINCLIENT_H
