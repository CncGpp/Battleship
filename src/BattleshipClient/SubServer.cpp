//
// Created by giuseppe on 27/12/16.
//

#include "SubServer.h"

#include <thread>
#include <Package.h>
#include "Wrapper.h"

int SubServer::sendRequest(int connfd, Package::Type type) {
    return (int)send(connfd,&type,sizeof(type), MSG_NOSIGNAL);
}
int SubServer::waitRequest(int connfd, Package::Type *type) {
    return (int)recv(connfd,type,sizeof(*type), 0);
}

int SubServer::sendReply(int connfd, Package::Status status) {
    return (int)send(connfd, &status, sizeof(status), MSG_NOSIGNAL);
}

int SubServer::sendPkg(int connfd, Package *pkg) {
    auto serialized = pkg->serialize();
    auto size = pkg->size();
    return (int)send(connfd,serialized.c_str(),size, MSG_NOSIGNAL);
}


template<typename T> int SubServer::waitPkg(int connfd, T *pkg) {
    auto size = pkg->size();
    char buff[size];
    auto n = recv(connfd,buff,size,0);
    *pkg = pkg->deserialize(buff);
    return (int) n;
}
/*TIPI CONSENTITI DALLA waitPkg*/
template int SubServer::waitPkg<ChallangeRequest>(int, ChallangeRequest*);
template int SubServer::waitPkg<MoveReply>(int, MoveReply*);
template int SubServer::waitPkg<MoveRequest>(int, MoveRequest*);



int SubServer::runSubServer() {
    /*Creo la socket*/
    cout << "Creo la socket in ascolto..." << endl;
    listenfd = _socket(AF_INET, SOCK_STREAM, 0);

    cout << "Riempio la struttura sockaddr_in..." << endl;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = 0;

    cout << "Effettuo il bind e mi metto in ascolto..." << endl;
    _bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    unsigned int addrlen = sizeof(servaddr);
    getsockname(listenfd, (struct sockaddr *) &servaddr, &addrlen);
    port = ntohs(servaddr.sin_port);

    _listen(listenfd, 10);
    return 0;
}

int SubServer::listenForInvite(string& foeUsername) {

    connfd = _accept(listenfd, NULL, NULL);

    Package::Type type;
    SubServer::waitRequest(connfd,&type);

    auto request = ChallangeRequest();
    SubServer::waitPkg(connfd,&request);

    foeUsername = request.username;

    return 0;
}


int SubServer::sendInviteReply(bool b) {
    auto pkg = ChallangeReply(b,"",0);
    return SubServer::sendPkg(connfd,&pkg);
}


int SubServer::sendMove(int x, int y) {
    MoveRequest pkg(x,y);
    SubServer::sendPkg(connfd,&pkg);

    auto replay = MoveReply();
    SubServer::waitPkg(connfd,&replay);

    return replay.result;
}


int SubServer::waitForMove(MoveRequest& m) {
    return SubServer::waitPkg(connfd,&m);
}

int SubServer::sendMoveReply(MoveReply::Result result) {
    MoveReply pkg(result);
    return SubServer::sendPkg(connfd,&pkg);
}


int SubServer::waitReady() {
    Package::Type type;
    waitRequest(connfd,&type);
    if(type != Package::Type::readyNotify) return -1;
    return 0;
}

int SubServer::sendReady() {
    return sendRequest(connfd,Package::Type::readyNotify);
}
