//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPSERVER_USERLIST_H
#define BATTLESHIPSERVER_USERLIST_H


#include <iostream>
#include <list>
using namespace std;
class UsersList {
    struct ListedUser {
        string username;
        string ip;
        short port;
        unsigned int victories, defeats;
    };

public:
    UsersList();
    size_t getSize();
    bool insert(string username, string ip, short port,unsigned int victories = 0, unsigned int defeats = 0);
    bool insert(string username, unsigned int victories = 0, unsigned int defeats = 0);
    void remove(string username);
    bool contain(string username);
    void removeAll() {list.clear();}
    void updateStats(string username, unsigned int victories, unsigned int defeats);
    short portOf(string username);
    string ipOf(string username);
    std::list<ListedUser> &getList() { return list; }

private:
    std::list<ListedUser> list;
    pthread_mutex_t mtx;
    bool _return(bool b) {pthread_mutex_unlock(&mtx); return b;}
};


#endif //BATTLESHIPSERVER_USERLIST_H
