//
// Created by giuseppe on 26/12/16.
//

#include <Wrapper.h>
#include "MainClient.h"

int MainClient::loginRequest(std::string username, std::string password, short port) {
    auto pkg = new LoginRequestPkg(username,password,port);
    Client::sendRequest(pkg->type);
    Client::sendPkg(pkg);
    delete pkg;

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status*100;
    }

    auto replay = new LoginReplyPkg();
    Client::waitPkg(replay);
    logged = replay->result;
    admin = replay->admin;
    delete replay;

 return 0;
}

int MainClient::onlineListRequest() {
    Client::sendRequest(Package::onlineListRequest);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status*100;
    }

    onlineList.getList().clear();

    size_t listSize = 0;
    recv(sockfd,&listSize,sizeof(listSize), 0);

    for(int i = 0; i < listSize; i++){
        auto pkg = OnlineUserPkg();
        Client::waitPkg(&pkg);
        onlineList.insert(pkg.username,pkg.victories,pkg.defeats);
    }

    return 0;
}

int MainClient::freeListRequest() {
    Client::sendRequest(Package::freeListRequest);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status*100;
    }

    freeList.getList().clear();

    size_t listSize = 0;
    recv(sockfd,&listSize,sizeof(listSize), 0);

    for(int i = 0; i < listSize; i++){
        auto pkg = FreeUserPkg();
        Client::waitPkg(&pkg);
        freeList.insert(pkg.username,pkg.victories,pkg.defeats);
    }

    return 0;
}

int MainClient::challangerRequest(string username, struct sockaddr_in *servaddr) {
    if(!isLogged()) return -1;

    auto pkg = ChallangeRequest(username);

    Client::sendRequest(Package::challangeRequest);
    Client::sendPkg(&pkg);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status*100;
    }

    auto replay = ChallangeReply();
    Client::waitPkg(&replay);

    if(replay.result and servaddr!= nullptr) {
        servaddr->sin_family = AF_INET;
        servaddr->sin_port = replay.port;
        inet_pton(AF_INET, replay.ip, &servaddr->sin_addr);
        return 1;
    }
    return 0;
}

int MainClient::sendNotify(Package::Type notify) {

    switch (notify){
        case Package::winNotify: break;
        case Package::loseNotify: break;
        default: return -1;
    }

    Client::sendRequest(notify);
    return 0;
}

int MainClient::matchRequest(string foeUsername) {
    if(!isLogged()) return -1;

    auto pkg = MatchRequest(foeUsername);
    Client::sendRequest(Package::matchRequest);
    Client::sendPkg(&pkg);
    return 0;
}

int MainClient::terminatedMatchesRequest() {
    if(!isLogged()) return -1;
    if(!isAdmin())  return -Package::unauthorized;

    Client::sendRequest(Package::terminatedMatchesRequest);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status;
    }

    size_t listSize = 0;
    recv(sockfd,&listSize,sizeof(listSize), 0);


    char buff[1024];
    cout << endl;
    cout << "LISTA DELLE PARTITE TERMINATE: " << endl;
    cout << "╭─────────────────────┬─────────────────────┬─────────────────────┬─────────────────────╮" << endl;
    sprintf(buff,"│ %-20s│ %-20s│ %-20s│ %-20s│","SFIDANTE","SFIDATO","DATA PARTITA","VINCITORE");
    cout << buff << endl;
    cout << "├─────────────────────┼─────────────────────┼─────────────────────┼─────────────────────┤" << endl;

    for(int i = 0; i < listSize; i++){
        auto replay = MatchPkg();
        Client::waitPkg(&replay);

        string winner = (replay.winner)? replay.username1:replay.username0;
        sprintf(buff,"│ %-20s│ %-20s│ %-20s│ %-20s│",replay.username0,replay.username1,replay.date,winner.c_str());
        cout << buff << endl;
    }
    cout << "╰─────────────────────┴─────────────────────┴─────────────────────┴─────────────────────╯" << endl;
    cout << endl;
    return 0;
}

int MainClient::inProgressMatchesRequest() {
    if(!isLogged()) return -1;
    if(!isAdmin())  return -Package::unauthorized;

    Client::sendRequest(Package::inProgressMatchesRequest);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status;
    }

    size_t listSize = 0;
    recv(sockfd,&listSize,sizeof(listSize), 0);

    char buff[1024];
    cout << endl;
    cout << "LISTA DELLE PARTITE IN CORSO: " << endl;
    cout << "╭─────────────────────┬─────────────────────┬─────────────────────╮" << endl;
    sprintf(buff,"│ %-20s│ %-20s│ %-20s│","SFIDANTE","SFIDATO","DATA PARTITA");
    cout << buff << endl;
    cout << "├─────────────────────┼─────────────────────┼─────────────────────┤" << endl;

    for(int i = 0; i < listSize; i++){
        auto replay = MatchPkg();
        Client::waitPkg(&replay);

        sprintf(buff,"│ %-20s│ %-20s│ %-20s│",replay.username0,replay.username1,replay.date);
        cout << buff << endl;
    }
    cout << "╰─────────────────────┴─────────────────────┴─────────────────────╯" << endl;
    cout << endl;
    return 0;
}

int MainClient::registeredUsersRequest() {
    if(!isLogged()) return -1;
    if(!isAdmin())  return -Package::unauthorized;

    Client::sendRequest(Package::registredUserRequest);

    Package::Status status;
    Client::waitReply(&status);

    switch(status){
        case Package::Status::success: break;
        default: return -status;
    }

    size_t listSize = 0;
    recv(sockfd,&listSize,sizeof(listSize), 0);

    char buff[1024];
    cout << endl;
    cout << "LISTA DEGLI UTENTI REGISTRATI: " << endl;
    cout << "╭─────────────────────┬──────────┬───────────╮" << endl;
    sprintf(buff,"│ %-20s│ %-9s│ %-10s│","USERNAME","VITTORIE","SCONFITTE");
    cout << buff << endl;
    cout << "├─────────────────────┼──────────┼───────────┤" << endl;

    for(int i = 0; i < listSize; i++){
        auto replay = OnlineUserPkg();
        Client::waitPkg(&replay);

        sprintf(buff,"│ %-20s│ %9d│ %10d│",replay.username,replay.victories,replay.defeats);
        cout << buff << endl;
    }
    cout << "╰─────────────────────┴──────────┴───────────╯" << endl;
    cout << endl;
    return 0;
}