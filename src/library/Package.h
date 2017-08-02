//
// Created by giuseppe on 17/12/16.
//

#ifndef SERVER_PACKAGE_H
#define SERVER_PACKAGE_H

#include <iostream>
#include <memory>
#include "string.h"
using namespace std;

class Package {
public:
     enum Type{
         null,
         loginRequest,
         loginReply,
         onlineListRequest,
         onlineListReply,
         freeListRequest,
         freeListReply,
         challangeRequest,
         challangeReply,
         moveRequest,
         moveReply,
         terminatedMatchesRequest,
         inProgressMatchesRequest,
         matchesReply,
         registredUserRequest,
         registredUserReply,
         matchRequest,

         readyNotify,
         winNotify,
         loseNotify

    };

    enum Status{
      success,
      error,
      unauthorized
    };


    Package() : type(null){}
    Package(const Type &type) : type(type) { }

    enum Type type;

    virtual size_t size() const { return 0;}

    virtual const string serialize(){return "";}
    Package deserialize(const char* src) {return Package();}

};

class LoginRequestPkg : public Package{
public:
    LoginRequestPkg() : Package(loginRequest) {}
    LoginRequestPkg(string username, string password, short port);

    static size_t pkgSize() {return sizeof(username) + sizeof(password) + sizeof(port);}
    size_t size() const {return pkgSize();}

    virtual const string serialize();
    LoginRequestPkg deserialize(const char* src);

    char username[20], password[20];
    short port;
};

class LoginReplyPkg : public Package{
public:
    LoginReplyPkg() : Package(loginReply) {}
    LoginReplyPkg(bool result, bool admin = 0) : Package(loginReply), result(result), admin(admin){}

    static size_t pkgSize() {return sizeof(result) + sizeof(admin);}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    LoginReplyPkg deserialize(const char* src);

    bool result;
    bool admin;
};




class OnlineUserPkg : public Package{
public:
    OnlineUserPkg() : Package(onlineListReply) {}
    OnlineUserPkg(string username, unsigned int victories, unsigned int defeats) : Package(onlineListReply),
                                                                                   victories(victories),
                                                                                   defeats(defeats) {
        strcpy(this->username,username.c_str());
    }

    static size_t pkgSize() {return sizeof(username) + sizeof(victories)*2;}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    OnlineUserPkg deserialize(const char*);


    char username[20];
    unsigned int victories, defeats;
};

class FreeUserPkg : public Package{
public:
    FreeUserPkg() : Package(freeListReply) {}
    FreeUserPkg(string username, unsigned int victories, unsigned int defeats) : Package(freeListReply),
                                                                                   victories(victories),
                                                                                   defeats(defeats) {
        strcpy(this->username,username.c_str());
    }

    static size_t pkgSize() {return sizeof(username) + sizeof(victories)*2;}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    FreeUserPkg deserialize(const char*);

    char username[20];
    unsigned int victories, defeats;
};

class ChallangeRequest : public Package{
public:
    ChallangeRequest() : Package(challangeRequest) {}
    ChallangeRequest(string username) : Package(challangeRequest){
        strcpy(this->username,username.c_str());
    }
    static size_t pkgSize() {return  sizeof(username);}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    ChallangeRequest deserialize(const char*);


    char username[20];
};

class ChallangeReply : public Package{
public:
    ChallangeReply() : Package(challangeReply) {}
    ChallangeReply(bool result, const string ip, short port) : Package(challangeReply),
                                                                result(result),
                                                                port(port) {
        strcpy(this->ip,ip.c_str());
    }

    static size_t pkgSize() {return sizeof(result) + sizeof(ip) + sizeof(port);}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    ChallangeReply deserialize(const char*);

    bool result;
    char ip[16];
    short port;
};



class MatchRequest : public Package{
public:
    MatchRequest() : Package(matchRequest) {}
    MatchRequest(string username) : Package(matchRequest){
        strcpy(this->username,username.c_str());
    }

    static size_t pkgSize() {return sizeof(username);}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    MatchRequest deserialize(const char*);

    char username[20];
};




class MoveRequest : public Package{
public:
    MoveRequest() : Package(moveRequest) {}
    MoveRequest(int x, int y) : Package(moveRequest), x(x), y(y) {}

    virtual const std::string serialize();
    MoveRequest deserialize(const char*);

    static size_t pkgSize() {return sizeof(x) + sizeof(y);}
    size_t size() const {return pkgSize();}
    int x,y;
};


class MoveReply : public Package{
public:
    enum Result{
        miss,
        hit,
        hitAndSunk
    };
    MoveReply() : Package(moveReply) {}
    MoveReply(enum Result result) :  Package(moveReply), result(result) {}
    virtual const std::string serialize();
    MoveReply deserialize(const char*);
    static size_t pkgSize() {return sizeof(result);}
    size_t size() const {return pkgSize();}
    enum Result result;
};



class MatchPkg : public Package{
public:
    MatchPkg() : Package(matchesReply){}
    MatchPkg(string username0, string username1, string date, bool winner) : Package(matchesReply){
        strcpy(this->username0,username0.c_str());
        strcpy(this->username1,username1.c_str());
        strcpy(this->date,date.c_str());
        this->winner = winner;
    }

    static size_t pkgSize() {return sizeof(username0)*2 + sizeof(date) + sizeof(winner);}
    size_t size() const {return pkgSize();}

    virtual const std::string serialize();
    MatchPkg deserialize(const char*);

    char username0[20], username1[20];
    char date[80];
    bool winner;
};



#endif //SERVER_PACKAGE_H
