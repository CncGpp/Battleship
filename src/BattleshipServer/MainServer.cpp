//
// Created by giuseppe on 27/12/16.
//

#include "MainServer.h"

int MainServer::start() {
    cout <<currentTime() << "Avvio del server in corso:" << endl;

    cout <<currentTime() << "Creo la socket..." << endl;
    listenfd = _socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(port);

    cout << currentTime() << "Effettuo il bind e mi metto in ascolto..." << endl;
    _bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    _listen(listenfd, 1024);


    list<thread> threadList;
    while(true){
        struct sockaddr_in clientaddr;
        socklen_t clientaddr_len;
        clientaddr_len = sizeof(clientaddr);
        connfd = _accept(listenfd, (struct sockaddr *)&clientaddr, &clientaddr_len);

        threadList.push_back(thread(&MainServer::manageClient, this, connfd, clientaddr));
    }
    for(auto& t : threadList) {t.join();}

    return 0;
}

void MainServer::manageClient(int _connfd, struct sockaddr_in  clientaddr) {

    char buffer[4097];
    inet_ntop(AF_INET,&clientaddr.sin_addr,buffer,sizeof(buffer));
    cout <<currentTime() << "Richiesta dall'host " << buffer << ":" << ntohs(clientaddr.sin_port) << endl;

    cout << "\t\tGestisco il client con sockfd = " << _connfd << endl;
    string connectedUsername = "";

    while(1){
        //Attendo di essere contattato dal Client per qualche richiesta
        Package::Type type;
        if(Server::waitRequest(_connfd,&type) <= 0) break;
        cout <<currentTime() << "(sockfd: " << _connfd <<") Mi ha richiesto un servizio: "<< type << endl;

        switch (type) {
            case Package::Type::loginRequest: {
                cout << "\t\tE' stata effettuata una richiesta di login" << endl;
                connectedUsername = loginReply(_connfd, clientaddr);
                break;
            }
            case Package::Type::onlineListRequest:{
                cout << "\t\tE' stata effettuata una richiesta per la lista degli utenti online" << endl;
                onlineListReply(_connfd);
                break;
            }
            case Package::Type::freeListRequest:{
                cout << "\t\tE' stata effettuata una richiesta per la lista degli utenti disponibili" << endl;
                freeListReply(_connfd);
                break;
            }
            case Package::Type::challangeRequest:{
                cout << "\t\tE' stata effettuata una richiesta di sfida!" << endl;
                challangeReply(_connfd);
                break;
            }
            case Package::Type::matchRequest: {
                string foeUsername = "";
                matchReply(_connfd, connectedUsername, foeUsername);
                cout << "\t\tIl giocatore " << connectedUsername << " è impegnato in una partita contro "
                << foeUsername << "!" << endl;
                break;
            }
            case Package::Type::winNotify: {
                cout << "\t\tIl giocatore " << connectedUsername << " ha vinto una partita!" << endl;
                registredList.increaseVictoriesOf(connectedUsername);
                auto ip = onlineList.ipOf(connectedUsername);
                auto port = onlineList.portOf(connectedUsername);
                freeList.insert(connectedUsername, ip, port);
                matches.terminateMatch(connectedUsername,true);
                break;
            }
            case Package::Type::loseNotify: {
                cout << "\t\tIl giocatore " << connectedUsername << " ha preso una partita!" << endl;
                registredList.increaseDefeatsOf(connectedUsername);
                auto ip = onlineList.ipOf(connectedUsername);
                auto port = onlineList.portOf(connectedUsername);
                freeList.insert(connectedUsername, ip, port);
                matches.terminateMatch(connectedUsername,false);
                break;
            }

            case Package::Type::terminatedMatchesRequest:
                cout << "\t\tE' stata effettuata una richiesta dei match terminati." << endl;
                sendTerminatedMatches(_connfd,connectedUsername);
                break;

            case Package::Type::inProgressMatchesRequest:
                cout << "\t\tE' stata effettuata una richiesta dei match in corso." << endl;
                sendInProgressMatches(_connfd,connectedUsername);
                break;

            case Package::Type::registredUserRequest:
                cout << "\t\tE' stata effettuata una richiesta degli utenti registrati." << endl;
                sendRegistredUsers(_connfd, connectedUsername);
                break;

            default:
                cout << "\t\tRichiesta sconosciuta " << endl;
                break;
        }
    }

    //Effettuo il logout dell'utente se la socket del client viene chiusa.
    if(matches.terminateMatch(connectedUsername,false) )                        //Se stava giocando ha perso...
        registredList.increaseDefeatsOf(connectedUsername);

    onlineList.remove(connectedUsername);                                       //Rimuovo dagli online
    freeList.remove(connectedUsername);                                         //Rimuovo eventualmente dai disponibili
    cout <<currentTime() << "(sockfd: " << _connfd <<") La connessione è terminata" << endl;
    close(_connfd);
    return;
}


string MainServer::loginReply(int _connfd, struct sockaddr_in clientaddr) {
    auto pkg = LoginRequestPkg();                                            //Creo il pacchetto da riempire
    Server::waitPkg(_connfd, &pkg);                                          //Attendo il pacchetto...

    Server::sendReply(_connfd, Package::success);                            //Confermo la richiesta

    bool result = registredList.validateLogin(pkg.username,pkg.password);     //Verifico se le credenziali sono corrette
    result = result and !onlineList.contain(pkg.username);                    //Verifico se l'utente è già online
    bool admin = registredList.isAdmin(pkg.username);                         //Verifico se è un admin

    auto replay = LoginReplyPkg(result,admin);                                //Creo un pacchetto di risposta
    Server::sendPkg(_connfd, &replay);                                        //e lo invio.

    if(!result) return "";                    //Se la verifica delle credenziali è andata male
                                              //La funzione finisce qui
                                              //Altrimenti aggiorno lo stato delle liste
    onlineList.insert(pkg.username,inet_ntoa(clientaddr.sin_addr),pkg.port);
    freeList.insert(pkg.username,inet_ntoa(clientaddr.sin_addr),pkg.port);

    return pkg.username;                     //Ritorno in fine l'username dell'utente appena loggato.
}

int MainServer::onlineListReply(int _connfd) {
    Server::sendReply(_connfd, Package::success);                           //Confermo la richiesta

    size_t listSize = onlineList.getSize();                                 //Invio il numero di elementi nella lista
    send(_connfd,&listSize,sizeof(listSize), 0);

    for(auto x : onlineList.getList()){                                     //Invio ogni elemento della lista...
        x.victories = registredList.getVictoryOf(x.username);               //Recupero le vittorie...
        x.defeats = registredList.getDefeatsOf(x.username);                 //Recupero le sconfitte...
        auto pkg = OnlineUserPkg(x.username,x.victories,x.defeats);         //Creo il pacchetto...
        Server::sendPkg(_connfd,&pkg);                                      //... Ed in fine lo invio.
    }

    return 0;
}

int MainServer::freeListReply(int _connfd) {
    Server::sendReply(_connfd, Package::success);                           //Confermo la richiesta

    size_t listSize = freeList.getSize();                                   //Invio il numero di elementi nella lista
    send(_connfd,&listSize,sizeof(listSize), 0);

    for(auto x : freeList.getList()){                                       //Invio ogni elemento della lista...
        x.victories = registredList.getVictoryOf(x.username);               //Recupero le vittorie...
        x.defeats = registredList.getDefeatsOf(x.username);                 //Recupero le sconfitte...
        auto pkg = FreeUserPkg(x.username,x.victories,x.defeats);           //Creo il pacchetto...
        Server::sendPkg(_connfd,&pkg);                                      //... Ed in fine lo invio.
    }

    return 0;
}

int MainServer::challangeReply(int _connfd) {
    auto pkg = ChallangeRequest();
    Server::waitPkg(_connfd, &pkg);

    Server::sendReply(_connfd, Package::success);

    //Verfico se l'username esiste, e se è disponibile...
    bool result = false;
    short port = 0;
    string ip = "0.0.0.0";
    if(!registredList.exist(pkg.username)){
        cout << "[ERRORE]: L'utente sfidato non esiste!" << endl;
    } else if(!onlineList.contain(pkg.username)){
        cout << "[ERRORE]: L'utente sfidato non è online!" << endl;
    } else if(!freeList.contain(pkg.username)){
        cout << "[ERRORE]: L'utente sfidato non è disponibile!" << endl;
    }
    else {  //Dunque se è disponibile...
        result = true;
        port = freeList.portOf(pkg.username);
        ip = freeList.ipOf(pkg.username);
    }

    auto replay = ChallangeReply(result, ip, port);                             //Creo il pacchetto di risposta...
    sendPkg(_connfd,&replay);                                                   //... e lo invio.

    return 0;
}

int MainServer::matchReply(int _connfd, const string &connectedUsername, string &foeUsername) {
    auto pkg = MatchRequest();
    Server::waitPkg(_connfd, &pkg);

    foeUsername = pkg.username;
    matches.insert(connectedUsername,foeUsername);

    freeList.remove(connectedUsername);
    freeList.remove(foeUsername);
    return 0;
}

int MainServer::sendTerminatedMatches(int _connfd, string connectedUsername) {
    Package::Status status = Package::success;

    if(!registredList.isAdmin(connectedUsername)){
        status = Package::unauthorized;
        Server::sendReply(_connfd,status);
        return 0;
    }
    Server::sendReply(_connfd, status);

    size_t listSize = matches.getTerminatedMatch().size();
    send(_connfd,&listSize,sizeof(listSize), 0);

    for(auto x: matches.getTerminatedMatch()){
        auto pkg = MatchPkg(x.username0,x.username1,x.date,x.winner);
        Server::sendPkg(_connfd,&pkg);
    }

    return 0;
}

int MainServer::sendInProgressMatches(int _connfd, string connectedUsername) {
    Package::Status status = Package::success;

    if(!registredList.isAdmin(connectedUsername)){
        status = Package::unauthorized;
        Server::sendReply(_connfd,status);
        return 0;
    }
    Server::sendReply(_connfd, status);

    size_t listSize = matches.getInPogressMatch().size();
    send(_connfd,&listSize,sizeof(listSize), 0);

    for(auto x: matches.getInPogressMatch()){
        auto pkg = MatchPkg(x.username0,x.username1,x.date,x.winner);
        Server::sendPkg(_connfd,&pkg);
    }
    return 0;
}

int MainServer::sendRegistredUsers(int _connfd, string connectedUsername) {
    if(!registredList.isAdmin(connectedUsername)){
        Server::sendReply(_connfd,Package::unauthorized);
        return 0;
    }
    Package::Status status = Package::success;
    Server::sendReply(_connfd, status);

    size_t listSize = registredList.getDbms().size();
    send(_connfd,&listSize,sizeof(listSize), 0);

    for(auto x: registredList.getDbms()){
        auto pkg = OnlineUserPkg(x.getUsername(),x.getVictories(),x.getDefeats());
        Server::sendPkg(_connfd,&pkg);
    }
    return 0;
}