//
// Created by giuseppe on 17/12/16.
//

#include <string.h>
#include <unistd.h>
#include "Package.h"
LoginRequestPkg::LoginRequestPkg(string username, string password, short port) : Package(loginRequest){
    strcpy(this->username,username.c_str());
    strcpy(this->password,password.c_str());
    this->port = port;
}
const string LoginRequestPkg::serialize() {
    char tmp[pkgSize()];
    unsigned int offset = 0;

    memset(tmp,'\0',pkgSize());
    memcpy(tmp,username,sizeof(username));                      offset+=sizeof(username);
    memcpy(tmp+offset,password, sizeof(password));              offset+=sizeof(password);
    memcpy(tmp+offset,&port, sizeof(port));

    return string(tmp,pkgSize());
}

LoginRequestPkg LoginRequestPkg::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(username,src,sizeof(username));                           offset+=sizeof(username);
    memcpy(password,src + offset, sizeof(password));                 offset+=sizeof(password);
    memcpy(&port,src + offset, sizeof(port));

    return LoginRequestPkg(username,password, port);
}


/* LOGIN REPLAY*/
const std::string LoginReplyPkg::serialize() {
    unsigned int offset = 0;
    size_t size = this->size();
    char tmp[size];

    memcpy(tmp,          &result, sizeof(result));           offset+=sizeof(result);
    memcpy(tmp + offset, &admin,  sizeof(admin) );
    return string(tmp,size);
}

LoginReplyPkg LoginReplyPkg::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&result, src + offset, sizeof(result));            offset+=sizeof(result);
    memcpy(&admin,  src + offset, sizeof(admin) );
    return LoginReplyPkg(result, admin);
}




/* USERS LISTS*/

const std::string OnlineUserPkg::serialize() {
    size_t size = this->size();
    unsigned int offset = 0;
    char tmp[size];

    memcpy(tmp + offset,&username,sizeof(username));                    offset+=sizeof(username);
    memcpy(tmp + offset,&victories,sizeof(victories));                  offset+=sizeof(victories);
    memcpy(tmp + offset,&defeats,sizeof(defeats));

    return string(tmp,size);
}

OnlineUserPkg OnlineUserPkg::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&username, src, sizeof(username));                     offset+=sizeof(username);
    memcpy(&victories, src + offset, sizeof(victories));          offset+=sizeof(victories);
    memcpy(&defeats, src + offset, sizeof(defeats));

    return OnlineUserPkg(username,victories,defeats);
}






const std::string FreeUserPkg::serialize() {
    size_t size = this->size();
    unsigned int offset = 0;
    char tmp[size];

    memcpy(tmp + offset,&username,sizeof(username));                    offset+=sizeof(username);
    memcpy(tmp + offset,&victories,sizeof(victories));                  offset+=sizeof(victories);
    memcpy(tmp + offset,&defeats,sizeof(defeats));

    return string(tmp,size);
}

FreeUserPkg FreeUserPkg::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&username, src, sizeof(username));                     offset+=sizeof(username);
    memcpy(&victories, src + offset, sizeof(victories));          offset+=sizeof(victories);
    memcpy(&defeats, src + offset, sizeof(defeats));

    return FreeUserPkg(username,victories,defeats);
}





const std::string ChallangeRequest::serialize() {
    return string(username,size());
}

ChallangeRequest ChallangeRequest::deserialize(const char *src) {
    memcpy(&username, src, sizeof(username));
    return  ChallangeRequest(username);
}




const std::string ChallangeReply::serialize() {
    size_t size = this->size();
    unsigned int offset = 0;
    char tmp[size];

    memcpy(tmp + offset,&result,sizeof(result));                    offset+=sizeof(result);
    memcpy(tmp + offset,ip,sizeof(ip));                           offset+=sizeof(ip);
    memcpy(tmp + offset,&port,sizeof(port));

    return string(tmp,size);
}

ChallangeReply ChallangeReply::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&result, src, sizeof(bool));                     offset+=sizeof(bool);
    memcpy(ip, src + offset, sizeof(ip));                   offset+=sizeof(ip);
    memcpy(&port, src + offset, sizeof(port));

    return ChallangeReply(result,ip,port);
}






const std::string MatchRequest::serialize() {
    return string(username,pkgSize());
}

MatchRequest MatchRequest::deserialize(const char *src) {
    memcpy(&username, src, sizeof(username));
    return  MatchRequest(username);
}





const std::string MoveRequest::serialize() {
    size_t size = this->size();
    unsigned int offset = 0;
    char tmp[size];

    memcpy(tmp + offset,&x,sizeof(x));                    offset+=sizeof(x);
    memcpy(tmp + offset,&y,sizeof(y));

    return string(tmp,size);
}

MoveRequest MoveRequest::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&x, src, sizeof(x));                     offset+=sizeof(x);
    memcpy(&y, src + offset, sizeof(y));

    return MoveRequest(x,y);
}





const std::string MoveReply::serialize() {
    size_t size = this->size();
    char tmp[size];

    memcpy(tmp ,&result,sizeof(result));

    return string(tmp,size);
}

MoveReply MoveReply::deserialize(const char *src) {
    memcpy(&result,src,sizeof(result));
    return MoveReply(result);
}





const std::string MatchPkg::serialize() {
    size_t size = this->size();
    unsigned int offset = 0;
    char tmp[size];

    memcpy(tmp + offset,&username0,sizeof(username0));                    offset+=sizeof(username0);
    memcpy(tmp + offset,&username1,sizeof(username1));                    offset+=sizeof(username1);
    memcpy(tmp + offset,&date,sizeof(date));                              offset+=sizeof(date);
    memcpy(tmp + offset,&winner,sizeof(winner));

    return string(tmp,size);
}

MatchPkg MatchPkg::deserialize(const char *src) {
    unsigned int offset = 0;

    memcpy(&username0, src + offset, sizeof(username0));                     offset+=sizeof(username0);
    memcpy(&username1, src + offset, sizeof(username1));                     offset+=sizeof(username1);
    memcpy(&date,      src + offset, sizeof(date));                          offset+=sizeof(date);
    memcpy(&winner,    src + offset, sizeof(winner));

    return MatchPkg(username0,username1,date,winner);
}

