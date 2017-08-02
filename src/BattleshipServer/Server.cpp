//
// Created by giuseppe on 26/12/16.
//

#include <Package.h>
#include "Server.h"

int Server::waitRequest(int connfd, Package::Type *type) {
    return (int)recv(connfd,type,sizeof(*type), 0);
}

int Server::sendReply(int connfd, Package::Status status) {
    return (int)send(connfd, &status, sizeof(status), MSG_NOSIGNAL);
}

int Server::sendPkg(int connfd, Package *pkg) {
    auto serialized = pkg->serialize();
    auto size = pkg->size();
    return (int)send(connfd,serialized.c_str(),size, MSG_NOSIGNAL);
}


template<typename T> int Server::waitPkg(int connfd, T *pkg) {
    auto size = pkg->size();
    char buff[size];
    auto n = recv(connfd,buff,size,0);
    *pkg = pkg->deserialize(buff);
    return (int) n;
}
/*TIPI CONSENTITI DALLA waitPkg*/
template int Server::waitPkg<LoginRequestPkg>(int, LoginRequestPkg*);
template int Server::waitPkg<ChallangeRequest>(int, ChallangeRequest*);
template int Server::waitPkg<MatchRequest>(int, MatchRequest*);