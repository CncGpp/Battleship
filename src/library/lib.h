//
// Created by giuseppe on 17/12/16.
//

#ifndef SERVER_LIB_H
#define SERVER_LIB_H
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

const std::string currentTime();
const std::string currentDate();

#endif //SERVER_LIB_H
