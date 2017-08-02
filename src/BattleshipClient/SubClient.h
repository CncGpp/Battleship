//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPCLIENT_SUBCLIENT_H
#define BATTLESHIPCLIENT_SUBCLIENT_H


#include "Client.h"

class SubClient : public Client{
public:
    SubClient(unsigned short timeoutInterval = 20) : timeoutInterval(timeoutInterval){}
    int invite(string username);

    int waitReady();
    int sendReady();

    int waitForMove(MoveRequest& m);
    int sendMove(int x, int y);
    int sendMoveReply(MoveReply::Result result);

protected:
    unsigned short timeoutInterval;
};


#endif //BATTLESHIPCLIENT_SUBCLIENT_H
