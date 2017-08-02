//
// Created by giuseppe on 26/12/16.
//

#ifndef BATTLESHIPCLIENT_CLIENT_H
#define BATTLESHIPCLIENT_CLIENT_H
#include <iostream>
#include "lib.h"
#include "Package.h"
using namespace std;

class Client {
public:
    Client(){}
    virtual ~Client(){ close(sockfd); }

    int connect(const char* ip, short port);                //Effettua la connessione al server
    int disconnect(){ return close(sockfd);};               //Efettua la disconnessione dal server

protected:
    int sockfd;

    int sendRequest(Package::Type type);                    //Invia una richiesta di tipo Package::Type al server
    int waitRequest(Package::Type* type);                   //Attende una richiesta dal server
    int waitReply(Package::Status* status);                 //Attende l'esito della richiesta
    int sendPkg(Package* pkg);                              //Invia un pacchetto dati al server
    template<typename T> int waitPkg(T* pkg);                                //Attende un pacchetto dati specifico (T) dal server

    int _select(short timeoutSeconds = 15);
};

#endif //BATTLESHIPCLIENT_CLIENT_H
