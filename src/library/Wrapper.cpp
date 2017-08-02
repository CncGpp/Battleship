//
// Created by giuseppe on 23/12/16.
//

#include "Wrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include<errno.h>

int _socket(int	family,	int	type,	int	protocol){
    int n;
    if(	(n	=	socket(family,	type,	protocol))	<	0)	{
        perror("Errore durante la creazione della socket.\n");
        exit(1);
    }
    return(n);
}

int _connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen){
    int n;
    if ( (n = connect(sockfd, servaddr, addrlen) ) < 0) {
        fprintf(stderr,"Errore durante la connessione.\n");
        exit(1);
    }

    return n;
}

int _inet_pton(int af, const char *src, void *dst){
    int n;
    if ((n=inet_pton(af, src, dst))< 0) {
        fprintf(stderr,"inet_pton error for %s\n", src);
        exit (1);
    }
    return n;
}


/******************************************************************************/


int _bind(int listenfd, const struct sockaddr *servaddr, socklen_t addrlen){
    if ( bind(listenfd, servaddr,  addrlen) < 0 ) {
        perror("Errore durante i bind\n");
        exit(1);
    }
    return 1;
}

int _listen(int listenfd, int backlog){
    if ( listen(listenfd, backlog) < 0 ) {
        perror("Errore durante il listen\n");
        exit(1);
    }
    return 1;
}

int _accept(int listenfd, struct sockaddr *addr, socklen_t* addrlen){
    int connfd;
    if ( ( connfd = accept(listenfd, addr, addrlen) ) < 0 ) {
        perror("Errore durante l'accept\n");
        exit(1);
    }
    return connfd;
}

