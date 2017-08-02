//
// Created by giuseppe on 23/12/16.
//

#ifndef SERVER_WRAPPER_H
#define SERVER_WRAPPER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<errno.h>


int _socket(int	family,	int	type, int protocol);
int _connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);
int _inet_pton(int af, const char *src, void *dst);

int _bind(int listenfd, const struct sockaddr *servaddr, socklen_t addrlen);
int _listen(int listenfd, int backlog);
int _accept(int listenfd, struct sockaddr *addr, socklen_t* addrlen);

#endif //SERVER_WRAPPER_H
