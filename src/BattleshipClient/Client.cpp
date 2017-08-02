//
// Created by giuseppe on 26/12/16.
//


#include <Wrapper.h>
#include "Client.h"

int Client::_select(short timeoutSeconds) {
    fd_set read_set;

    struct timeval timeout;
    timeout.tv_sec = timeoutSeconds; // Time out
    timeout.tv_usec = 0;

    FD_ZERO(&read_set);
    FD_SET(sockfd, &read_set);

    int r=select(sockfd+1, &read_set, NULL, NULL, &timeout);

    if( r<0 ) {
        cerr << "[SELECT ERROR]: Errore durante la select" << endl;
        exit(errno);
    }
    if( r==0 ) {
//        cerr << "[TIMEOUT]: Timeout durante la request" << endl;
        return -1;
    }
    return 0;
}



int Client::connect(const char *ip, short port) {
    /*La funzione effettua solamente la connessione con il server socket */
    sockfd = _socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(port);
    _inet_pton(AF_INET, ip, &servaddr.sin_addr);

    return _connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
}



int Client::sendRequest(Package::Type type) {
    return (int)send(sockfd,&type,sizeof(type), MSG_NOSIGNAL);
}

int Client::waitRequest(Package::Type *type) {
    return (int)recv(sockfd,type,sizeof(*type), 0);
}

int Client::waitReply(Package::Status* status) {
    return (int)recv(sockfd, status, sizeof(*status),0);
}

int Client::sendPkg(Package *pkg) {
    auto serialized = pkg->serialize();
    auto size = pkg->size();
    return (int)send(sockfd,serialized.c_str(),size, MSG_NOSIGNAL);
}


template<typename T> int Client::waitPkg(T *pkg) {
    auto size = pkg->size();
    char buff[size];
    auto n = recv(sockfd,buff,size,0);
    *pkg = pkg->deserialize(buff);
    return (int) n;
}

/*TIPI CONSENTITI DALLA waitPkg*/
template int Client::waitPkg<LoginReplyPkg>(LoginReplyPkg*);
template int Client::waitPkg<OnlineUserPkg>(OnlineUserPkg*);
template int Client::waitPkg<FreeUserPkg>(FreeUserPkg*);
template int Client::waitPkg<ChallangeReply>(ChallangeReply*);

template int Client::waitPkg<MoveReply>(MoveReply*);
template int Client::waitPkg<MoveRequest>(MoveRequest*);

template int Client::waitPkg<MatchPkg>(MatchPkg*);