//
// Created by giuseppe on 27/12/16.
//

#include "RegistredUsers.h"
RegistredUsers::RegistredUsers(){
    addUser(User("a", "a", 0 , 0 ));
    addUser(User("b", "b", 0 , 0 ));
    addUser(User("c", "c", 0 , 0 ));

    addUser(User("utente1", "utente1", 10, 5 ));
    addUser(User("utente2", "utente2", 4 , 4 ));
    addUser(User("utente3", "utente3", 6 , 3 ));
    addUser(User("utente4", "utente4", 1 , 9 ));
    addUser(User("utente5", "utente5", 12, 14));
    addUser(User("utente6", "utente6", 1 , 0 ));

    addUser(User("giuseppe"  , "12345678", 3 , 1 ));
    addUser(User("francesco" , "12345678", 3 , 1 ));

    addUser(User("admin"     , "admin", 0,0,true));
}

bool RegistredUsers::validateLogin(std::string username, std::string password) {
    auto x = dbms.find(User(username));

    if( x != dbms.end() ) return x->getPassword() == password;
    else return false;
}

void RegistredUsers::increaseVictoriesOf(const std::string &username) {
    std::set<User>::iterator x = dbms.find(User(username));
    if( x == dbms.end() ) return;
    else x->increaseVictories();
}

void RegistredUsers::increaseDefeatsOf(const std::string &username) {
    std::set<User>::iterator x = dbms.find(User(username));
    if( x == dbms.end() ) return;
    else x->increaseDefeats();
}

unsigned int RegistredUsers::getVictoryOf(const std::string &username) const {
    std::set<User>::iterator x = dbms.find(User(username));
    if( x == dbms.end() ) return 0;
    else x->getVictories();
}

unsigned int RegistredUsers::getDefeatsOf(const std::string &username) const {
    std::set<User>::iterator x = dbms.find(User(username));
    if( x == dbms.end() ) return 0;
    else x->getDefeats();
}

bool RegistredUsers::isAdmin(const std::string &username) const {
    std::set<User>::iterator x = dbms.find(User(username));
    if( x == dbms.end() ) return false;
    else x->isAdmin();
}