//
// Created by giuseppe on 27/12/16.
//

#include "SubClient.h"

int SubClient::invite(string username) {
    auto pkg = ChallangeRequest(username);                        //Creo un nuovo pacchetto per inviare la

    Client::sendRequest(Package::Type::challangeRequest);
    Client::sendPkg(&pkg);

    if(Client::_select(timeoutInterval) == -1) return -1;         //Se dopo timeoutInterval sec il descrittore non è pronto...

    auto replay = ChallangeReply();
    Client::waitPkg(&replay);

    return replay.result;
}


int SubClient::sendMove(int x, int y) {
    MoveRequest pkg(x,y);
    Client::sendPkg(&pkg);

    auto replay = MoveReply();
    Client::waitPkg(&replay);

    return replay.result;
}


int SubClient::waitForMove(MoveRequest& m) {
    return Client::waitPkg(&m);
}

int SubClient::sendMoveReply(MoveReply::Result result) {
    MoveReply pkg(result);
    return Client::sendPkg(&pkg);
}


int SubClient::waitReady() {
    Package::Type type;
    Client::waitRequest(&type);
    if(type != Package::Type::readyNotify) return -1;
    return 0;
}

int SubClient::sendReady() {
    return Client::sendRequest(Package::Type::readyNotify);
}