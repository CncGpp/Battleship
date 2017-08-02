//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPSERVER_REGISTREDUSERS_H
#define BATTLESHIPSERVER_REGISTREDUSERS_H

#include <set>
#include <iostream>
using namespace std;

class User{
    friend bool operator<(const User &u1, const User &u2) {return u1.username < u2.username;}
public:
    User(const std::string &username,
         const std::string &password = "",
         unsigned int victories = 0,
         unsigned int defeats = 0,
         bool admin = false
    ) : username(username), password(password), victories(victories), defeats(defeats), admin(admin) { }


    std::string getUsername() const {return username;}
    std::string getPassword() const {return password;}
    unsigned int getVictories() const { return victories;}
    unsigned int getDefeats() const { return defeats;}
    void increaseVictories() const {victories++;}
    void increaseDefeats() const {defeats++;}
    bool isAdmin() const { return admin; }

private:
    std::string username, password;
    mutable unsigned int victories, defeats;
    bool admin;
};



class RegistredUsers {
public:
    RegistredUsers();
    bool addUser(const User &u){return dbms.insert(u).second; }
    bool exist(const std::string &username){ return dbms.find(User(username)) != dbms.end();}
    bool validateLogin(std::string username, std::string password);
    void increaseVictoriesOf(const std::string &username);
    void increaseDefeatsOf(const std::string &username);
    unsigned int getVictoryOf(const std::string &username) const;
    unsigned int getDefeatsOf(const std::string &username) const;
    bool isAdmin(const std::string &username) const;

    const std::set<User>& getDbms() const {return dbms;}

    void print(){
        for(auto x : dbms) std::cout << x.getUsername() << std::endl;
    }
private:
    std::set<User> dbms;
};



#endif //BATTLESHIPSERVER_REGISTREDUSERS_H
