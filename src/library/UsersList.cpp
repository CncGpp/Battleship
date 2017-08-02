//
// Created by giuseppe on 27/12/16.
//

#include "UsersList.h"

UsersList::UsersList() {
    mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_unlock(&mtx);
}

bool UsersList::insert(string username, string ip, short port, unsigned int victories, unsigned int defeats) {
    pthread_mutex_lock(&mtx);

    if(this->contain(username)) return _return(false);
    const struct ListedUser temp = {username,ip,port, victories, defeats};
    list.push_back(temp);
    return _return(true);
}

bool UsersList::insert(string username, unsigned int victories, unsigned int defeats) {
    return insert(username,"",0,victories,defeats);
}


void UsersList::remove(string username) {
    pthread_mutex_lock(&mtx);
    auto it = list.begin();
    for(it; it != list.end(); it++)
        if(it->username == username) {
            list.erase(it);
            pthread_mutex_unlock(&mtx);
            return;
        }
    pthread_mutex_unlock(&mtx);
}

bool UsersList::contain(string username) {
    for(auto x : list) if(x.username == username) return _return(true);
    return _return(false);
}

void UsersList::updateStats(string username, unsigned int victories, unsigned int defeats) {
    pthread_mutex_lock(&mtx);
    for(auto x : list)
        if(x.username == username) {
            x.victories = victories;
            x.defeats = defeats;
            break;
        }
    pthread_mutex_unlock(&mtx);
}

short UsersList::portOf(string username) {
    pthread_mutex_lock(&mtx);
    for(auto x : list)
        if(x.username == username){
            pthread_mutex_unlock(&mtx);
            return x.port;
        }


}

string UsersList::ipOf(string username) {
    pthread_mutex_lock(&mtx);
    for(auto x : list)
        if(x.username == username){
            pthread_mutex_unlock(&mtx);
            return x.ip;
        }

}

size_t UsersList::getSize() {
    pthread_mutex_lock(&mtx);
    size_t s = list.size();
    pthread_mutex_unlock(&mtx);
    return s;
}


