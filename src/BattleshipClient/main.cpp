#include <iostream>
#include <thread>
#include <stdlib.h>
#include <bits/stl_numeric.h>
#include "MainClient.h"
#include "GUI/GUI.h"
#include "GUI/Terminal.h"
#include "GUI/Graphics/Box.h"
#include "SubServer.h"
#include "SubClient.h"
#include "GUI/Graphics/Ship.h"
#include "GUI/Graphics/Mark.h"
#include "GUI/Graphics/TextBox.h"
#include "GUI/Board.h"

using namespace std;


/// CONFIGURAZIONE
const unsigned int UPDATE_INTERVAL = 10;                        //Intervallo fra l'aggiornamento delle liste untente
const vector<int> shipToPlaceSize = {4,3,3,2,2,2,1};            //vettore di lunghezze delle navi da posizionare.
                                                                // [#nave] -> [size]

/// VARIABILI GLOBALI
GUI gui;                                //Contiene le funzioni base per il disegno dello schermo.

MainClient mc;                          //Client principale che si connette al server di gioco.
SubServer ss;                           //Sotto server eseguito dal client di gioco per poter accettare sfide.
SubClient sc;                           //Sotto client per connettersi ai sottoserver degli altri giocatori.

string username, foeUsername;           //Username dell'utente connesso, e dell'untente che sta sfidando.

bool isPlaying = false;                 //Se l'utente sta in fase di gioco o meno...
bool chosen = false;                    //Se l'utente ha scelto o meno lo sfidante dalla lobby.
bool isHost = true;                     //Se l'utente è l'host o il server della partita.

pthread_mutex_t lobbyReady = PTHREAD_MUTEX_INITIALIZER;    //Sincronizzazione fra thread lobby e thread del subserver
pthread_mutex_t mtxLists = PTHREAD_MUTEX_INITIALIZER;      //Accesso in muta esclusione alle liste degli utenti...
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;           //Sincronizzazione fra il thread in esecuzione...
pthread_t lobbyTID;                     //TID del thread che stampa la scena del lobby
pthread_t listsTID;                     //TID del thread che richiede le partite



///PROTOTIPI
void* lobbyScene(void*);
void* listsRequest(void*);
void startSubServer(SubServer* ss);

bool inviteReplyScene(string& foeUsername);

void playGameScene();
void placeShipScene(Board& board);
void battleScene(Board& board);

void statsScene();





int main(int argc,  char* argv[]) {

    string ip = "127.0.0.1";                                        //ip utilizzato di default dal client nella connessione
    short port = 1024;                                              //porta utilizzata di default dal client durante la connessione

    int c;
    while((c = getopt(argc, argv, "a:p:")) != -1){
        switch(c){
            case 'a':
                ip = string(optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s [-a] [IP]  [-p] [port]\n", argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }


    /* CONNESSIONE AL SERVER */
    mc.connect(ip.c_str(), port);                                       //Mi connetto al server di gioco.
    thread t = thread(&startSubServer, &ss);                            //Eseguo in background il sottoserver, per ascoltare
    t.detach();                                                         //eventuali richieste di sfida.

    do {
        string password;
        gui.loginScene(username, password);                             //Mostro la schermata di login
        mc.loginRequest(username, password, ss.getPort());              //Invio le credenziali al server di gioco,
        if (!mc.isLogged()) gui.loginErrorScene();                      //In caso di errore mostro un popup
    } while (!mc.isLogged());


    /* ADMIN ONLY*/
    if(mc.isAdmin()) statsScene();                                      //Se ho i privilegi di admin mostro la schermata
                                                                        //relativa alle statistiche.

    /* LOOP PRINCIPALE*/
    while(1) {
        pthread_mutex_unlock(&mtx);                                     //Sblocco il mutex, sbloccando il thread del subserver
        foeUsername == "";
        chosen = false;

        pthread_create(&listsTID,NULL,listsRequest,NULL);               //Creo il thread che effettuerà le richieste delle liste

        pthread_create(&lobbyTID, NULL, lobbyScene, NULL);              //Creo il thread che disegnerà la lobby
        pthread_join(lobbyTID, NULL);                                   //Attendo che termini o sia cancellato...

        // Se al termine della lobbyScene la variabile chosen == true vuol dire che ho scelto un giocatore da sfidare
        if(chosen){
            struct sockaddr_in foeServaddr;
            if( mc.challangerRequest(foeUsername, &foeServaddr) == 0){       //Richiedo i suoi dati al server di gioco
                gui.messageBox("Giocatore non disponibile!", Color::Red);
                getch();
                continue;
            }

            char _buffer[20];
            inet_ntop(AF_INET,&foeServaddr.sin_addr, _buffer,sizeof(_buffer));

            gui.messageBox("In attesa del tuo sfidante...", Color::Yellow);

            sc.connect(_buffer,foeServaddr.sin_port);                        //Ricevuti i dati richiedo una connessione
            isPlaying = sc.invite(username) == 1;                            //E invio una richiesta di gioco...

            // Se la richiesta è stata accettata, mi preparo per giocare...
            if (isPlaying) {
                mc.matchRequest(foeUsername);                               //Notifico al server che siamo non disponibili

                pthread_mutex_lock(&mtxLists);
                pthread_cancel(listsTID);                                   //Interrompo la richiesta delle liste
                pthread_mutex_unlock(&mtxLists);

                sc.sendReady();                                             //Avviso che sono pronto a iniziare una partita
                sc.waitReady();                                             //Attendo una risposta dall'avversario

                playGameScene();                                            //Gioco la partita
                isPlaying = false;
                sc.disconnect();                                            //Finito di giocare mi disconnetto.
            } else{
                sc.disconnect();                                            //... Altrimenti termino la connessione
                gui.messageBox("L'avversario ha rifiutato l'invito!", Color::Red);
                getch();
            }

        }


        pthread_mutex_lock(&mtx);                                           //Blocco il mutex.
        //In questo modo, nel caso, attendo che il thread del subserver abbia finito prima di ricominciare
    }


    return 0;
}


void startSubServer(SubServer* ss){
    ss->runSubServer();                                   //Faccio partire il sub server

    while(1){
        pthread_mutex_lock(&lobbyReady);                  //Mi assicuro che la lobby sia caricata e disegnata
        ss->listenForInvite(foeUsername);                 //Mi metto in attesa di un invito...

        pthread_mutex_lock(&mtx);                         //Blocco il mutex per avere priorità sul thread principale

        pthread_cancel(lobbyTID);                         //Cancello il thread lobbyScene
        pthread_mutex_unlock(&mtxLists);                  //E sblocco un eventuale mutex non ancora riaperto.

        bool reply = inviteReplyScene(foeUsername);       //Mostro la schermata per accettare o meno la l'invito
        ss->sendInviteReply( reply );                     //Invio la mia risposta

        if(reply) {                                       //Se l'esito è positivo mi preparo a giocare
            ss->sendReady();                              //Avviso che sono pronto a iniziare una partita
            auto x = ss->waitReady();                     //Attendo l'avversario con la sua risposta...

            if(x < 0) {                                   //Se la connessione è stata interrotta termino la partita.
                gui.messageBox("L'avversario non è più disponibile a giocare!!!", Color::Yellow);
                getch();
                pthread_mutex_unlock(&mtx);               //Sblocco il mutex e ricomincio
                continue;
            }

            isPlaying = true;
            isHost = false;                               //Gioco come server e non come host
            playGameScene();                              //Gioco...
            isPlaying = false;
            isHost = true;
        }

        pthread_mutex_unlock(&mtx);                       //Sblocco il mutex e ricomincio

    }
}

void* listsRequest(void* arg){
    while(1){
        if(isPlaying) return 0;                           //Se sto giocando già non ho bisogno delle liste... termino
        pthread_mutex_lock(&mtxLists);                    //Accedo in mutua esclusione alle liste e le aggiorno
        mc.onlineListRequest();                           //Richiedo la lista degli utenti online
        mc.freeListRequest();                             //E quella degli utenti disponibili a giocare
        pthread_mutex_unlock(&mtxLists);
        sleep(UPDATE_INTERVAL);
    }

    return 0;
}

void* lobbyScene(void* a){
    if(isPlaying) return nullptr;                                       //Se sto giocando esco.

    // Inizializzo il terminale
    gui.clearScreen();
    gui.initScreenSize();
    auto lines = gui.lines;
    auto column = gui.column;
    auto boxWidth = (column/3+5) < 34 ? 34 : column/3+5;                //Larghezza del box, varia in base al terminale

    // Costruisco le scatole che dovranno contenere rispettivamente la lista dei
    // giocatori online e disponibili a giocare.
    auto onlinePlayerBox =  Box(column/2-boxWidth-2,8,boxWidth,lines - 12);
    onlinePlayerBox.setTitle("Giocatori Online");
    onlinePlayerBox.setAttrColor(Color::Dim);
    onlinePlayerBox.setFilled(true);

    auto freePlayerBox =  Box(column/2 + 2,8,boxWidth,lines-12);
    freePlayerBox.setTitle("Giocatori disponibili");
    freePlayerBox.setAttrColor(Color::Bright);
    freePlayerBox.setFilled(true);

    //STAMPO LE ISTRUZIONI
    string message0 = "Utilizzare le freccette direzionali ↑ oppure ↓ per";
    string message1 = "muoversi nella lista, INVIO per confermare e Q per uscire.";
    string message2 = "La lista dei giocatori online verrà aggiornata ogni " + to_string(UPDATE_INTERVAL) + " secondi";
    string message3 = "premere un qualsiasi tasto per aggiorare la visualizzazione.";
    printXY(column/2-(int)message0.size()/2,3,message0);
    printXY(column/2-(int)message1.size()/2,4,message1);

    auto outline = Box(0,0,column,lines);
    outline.setTitle(username);
    outline.setAttrColor(Color::Dim);

    printXY(outline.getX() + column/2-(int)message2.size()/2,
            outline.getY()+outline.getHeight() - 3 ,
            message2);
    printXY(outline.getX() + column/2-(int)message3.size()/2,
            outline.getY()+outline.getHeight() - 2 ,
            message3);


    //Stampo le liste nelle box
    auto selectedColor = Color::color(Color::Reverse ,Color::Default, Color::Black);
    auto defaultColor = Color::DEFAULT;
    int userSelected = 0;                                           //Numero in lista, dell'utente selezionato
    string usernameSelected = "";                                   //Username di tale utente

    //Fin che NON sto giocando...
    while(!isPlaying) {
        outline.draw();                                             //Disegno il bordo, e le scatole
        onlinePlayerBox.draw();
        freePlayerBox.draw();

        pthread_mutex_lock(&mtxLists);                              //Accedo in mutua esclusione alle liste per stamparle

        int i = 0;
        for(auto x: mc.getOnlineList().getList()){
            char tmp[100];
            sprintf(tmp,"%-20s %3d:%-3d", x.username.c_str(),x.victories,x.defeats);
            printXY(onlinePlayerBox.getX() -13 + onlinePlayerBox.getWidth()/2,
                    onlinePlayerBox.getY()+1+i,
                    tmp
            );
            i++;
        }

        i = 0;
        for(auto x: mc.getFreeList().getList()){
            if(x.username == username) {cout<<endl; continue;}
            char tmp[100];
            sprintf(tmp,"%-20s %3d:%-3d", x.username.c_str(),x.victories,x.defeats);

            auto color = defaultColor;
            auto text  = string(tmp);
            if(i == userSelected){
                color = selectedColor;
                text  = "[ " + text + " ]";
                usernameSelected = x.username;
            }

            cprintXY(freePlayerBox.getX() - text.length()/2 + freePlayerBox.getWidth()/2,
                     freePlayerBox.getY()+1+i,
                     color,
                     text
            );
            i++;
        }

        pthread_mutex_unlock(&mtxLists);                          //Fine della mutua esclusione
        pthread_mutex_unlock(&lobbyReady);                        //Segnalo che la lobby è pronta ed è stata disegnata

        /*Seleziono l'utente con cui voglio giocare*/
        switch (getch()){
            case 'q': case 'Q':{                                            //Termina la partita
                gui.clearScreen();
                gui.messageBox("Grazie per aver giocato!",Color::Cyan);
                setCursorPos(0,column);
                mc.disconnect();
                sc.disconnect();
                exit(1);
            }
            case UP_ARROW:
                if(userSelected > 0) userSelected--;
                break;
            case DOWM_ARROW:
                if(userSelected < i -1) userSelected++;
                break;
            case ENTER:
                if(usernameSelected == "") break;
                chosen = true;
                foeUsername = usernameSelected;
                return 0;
            default: chrono::milliseconds(100);
                break;
        }

    }

    return 0;
}

bool inviteReplyScene(string& foeUsername){
    //pthread_mutex_lock(&mtx);

    auto myColor = Color::color(Color::Bright, Color::Blue, Color::Default);
    auto lines = gui.lines;
    auto column = gui.column;

    char text1[1024];
    char text2[] = "Accettare?  S/N";
    sprintf(text1, "Il giocatore < %s > ti ha sfidato!\0", foeUsername.c_str());
    int text1Size = (int)strlen(text1);
    int text2Size = (int)strlen(text2);
    int spacing = 4;

    auto warning = Box(column/2-text1Size/2,lines/2-3, text1Size +2 +spacing ,7);
    warning.setFilled(true);
    warning.setAttrColor(Color::Bright);
    warning.setForeColor(Color::Blue);
    warning.draw();

    cprintXY(warning.getX() + warning.getWidth()/2 - text1Size/2, warning.getY()+2,myColor,text1);
    cprintXY(warning.getX() + warning.getWidth()/2 - text2Size/2, warning.getY()+4,myColor,text2);

    while(1){
        char c = getch();
        if(c == 's' or c == 'S') {/*pthread_mutex_unlock(&mtx);*/ return true; }
        if(c == 'n' or c == 'N') {/*pthread_mutex_unlock(&mtx);*/ return false; }
    }

    return false;
}



void playGameScene(){
    auto lines = gui.lines;
    auto column = gui.column;

    auto myBoard = Board(4,2,9);                                        //Creo la mia board di gioco
    myBoard.grid.setX( column/2 - myBoard.grid.getWidth() - 5);         //Centro la board

    placeShipScene(myBoard);                                            //Chiamo la funzione per posizionare le navi

    gui.messageBox("In attesa dell'altro giocatore...", Color::Yellow); //E attendo che l'avversario finisca il posizionamento
    if(isHost){
        sc.sendReady();                                                 //Se sono l'host invio e ricevo dal subclient
        sc.waitReady();
    } else {
        ss.sendReady();                                                 //altrimenti invio e ricevo dal subserver
        ss.waitReady();
    }

    battleScene(myBoard);                                               //Fattò ciò mostro la schermata di battaglia.
}


/* LE SEGUANTI FUNZIONI CHIAMANO I METODI DEL SUB SERVER O DEL SUB CLIENT A SECONDA SE IL GIOCATORE E' STATO SFIDATO
 * O HA SFIDATO UN ALTRO GIOCATORE (ovvero se è server o host della partita)*/
int sendMove(int x, int y){
    if(isHost)
        return sc.sendMove(x,y);
    else
        return ss.sendMove(x,y);
}

int sendMoveReply(MoveReply::Result result){
    if(isHost)
        return sc.sendMoveReply(result);
    else
        return ss.sendMoveReply(result);
}

int waitMove(MoveRequest& m){
    if(isHost)
        return sc.waitForMove(m);
    else
        return ss.waitForMove(m);
}



void battleScene(Board& myBoard){
    // Inizializzo lo schermo
    gui.clearScreen();
    gui.initScreenSize();
    auto lines = gui.lines;
    auto column = gui.column;

    // Stampo il bordo
    auto outline = Box(0,0,column,lines);
    outline.setTitle(username);
    outline.setAttrColor(Color::Dim);
    outline.draw();

    // Stampo le board, la mia e quella avversaria
    myBoard.draw();
    auto enemyBoard = Board(column/2 +5 ,2,9);
    int size = enemyBoard.grid.getSize();

    // Disegno la log box
    auto logHeight = lines-size*2-1 -2;
    auto log = TextBox(column/2-column/4-3,lines - logHeight , column/2+6, logHeight-1);
    log.setTitle("Log");


    //inizializzo la posizione iniziali del cursore sulla board avversaria
    int _x = size / 2;
    int _y = _x;
    auto mark = Mark(enemyBoard.getXforCell(_x), enemyBoard.getYforCell(_y), Mark::selected);

    //Calcolo i colpi a segno necessari per vincere la partita...
    const int hitNeeded = std::accumulate(shipToPlaceSize.begin(), shipToPlaceSize.end(), 0);
    int hitToWin = hitNeeded;
    int hitToLose = hitNeeded;
    bool myTurn = !isHost;                  //Il giocatore sfidato comincia per primo!

    auto boldColor = Color::color(Color::Bright,Color::Default,Color::Default);
    if(myTurn) log.push(csprint(boldColor,"E' il tuo turno!"));

    //Fin che vinco o perdo la partita...
    while(hitToLose > 0 and hitToWin > 0){
        const char A = 'A';                 //Lettera di partenza, usata come base nelle coordinate della griglia
        char tmp[column];                   //Buffer usato per la creazione dei messagi del log

        /* DISEGNO GLI ELEMENTI DELLA SCENA*/
        mark.setX(enemyBoard.getXforCell(_x));
        mark.setY(enemyBoard.getYforCell(_y));
        enemyBoard.draw();
        myBoard.draw();
        mark.draw();
        log.draw();

        /* FIN CHE E' IL TURNO AVVERSARIO ASPETTO UNA SUA MOSSA...*/
        while(!myTurn){
            MoveRequest m;
            log.push("E' il turno di < " + foeUsername + " > attendi...");   log.draw();

            //Attendo che invii una mossa
            if(waitMove(m) <= 0) {
                hitToWin = 0;
                gui.messageBox("< " + foeUsername + " > si è disconensso!", Color::Blue);
                getch();
                break;
            }

            auto result = myBoard.hit(m.x,m.y);         //Dalle coordinate inviate colpisco la mia board
            sendMoveReply(result);                     //E invio il risultato dell'attacco

            sprintf(tmp,"%s attacca in %c%d!\0",csprint(boldColor,foeUsername.c_str()).c_str(),A+m.y,m.x);
            log.push(tmp);  log.draw();

            if(result == MoveReply::miss){             //Se l'avversario sbaglia
                myTurn = true;                          //E' il mio turno

                log.push(csprint(boldColor,"E' il tuo turno!"));
                log.draw();
            }
            else if(--hitToLose <= 0) break;            //Altrimenti diminuisco i colpi necessari a perdere
            myBoard.draw();
        } //Inner while


        //Se è il mio turno allora, attendo l'input per muovere il cursore o attaccare
        if(hitToLose <= 0 or hitToWin <= 0) break;
        switch (getch()){
            case LEFT_ARROW:
                if (_x > 0) _x--; break;
            case DOWM_ARROW:
                if(_y < size -1) _y++; break;
            case UP_ARROW:
                if(_y > 0) _y--; break;
            case RIGHT_ARROW:
                if(_x < size -1) _x++; break;
            case ENTER: {
                if(enemyBoard.isMarked(_x,_y)){                                 //Se il punto colpito era già marcato..
                    sprintf(tmp,"Hai già colpito il punto %c%d!\0",A+_y,_x);
                }
                else {
                    int status = sendMove(_x,_y);                               //Invio la mossa attendendo il risultato

                    if(status == MoveReply::miss){
                        Mark m(Mark::miss);
                        enemyBoard.addMark(_x, _y, m);
                        sprintf(tmp, "Hai colpito il punto %c%d \t ACQUA!\0", A + _y, _x);
                        myTurn = false;
                    } else

                    if(status == MoveReply::hit){
                        Mark m(Mark::hit);
                        enemyBoard.addMark(_x, _y, m);
                        sprintf(tmp, "Hai colpito il punto %c%d \t COLPITA!\0", A + _y, _x);
                        hitToWin--;
                        myTurn = true;

                    } else

                    if(status == MoveReply::hitAndSunk){
                        Mark m(Mark::hit);
                        enemyBoard.addMark(_x, _y, m);
                        sprintf(tmp, "Hai colpito il punto %c%d \t COLPITA E AFFONDATA!\0", A + _y, _x);
                        hitToWin--;
                        myTurn = true;
                    }
                    else {
                        /*Se l' status non è fra quelli disponibili, la connessione è stata chiusa!*/
                        myTurn = false;
                        continue;
                    }

                }
                log.push( string(tmp) );
                break;
            }
            default: break;
        }

    } // Outer while

    myBoard.draw();
    enemyBoard.draw();
    if(hitToWin == 0){
        gui.messageBox("COMPLIMENTI, HAI VINTO!", Color::Green);
        mc.sendNotify(Package::winNotify);                              //Invio al server la mia vittoria
    } else{
        gui.messageBox("SEI STATO SCONFITTO!",  Color::Red);
        mc.sendNotify(Package::loseNotify);                             //Invio al server la mia sconfitta
    }

    getch();
}



void placeShipScene(Board& board){
    //Inizializzo lo schermo
    gui.clearScreen();
    gui.initScreenSize();
    auto lines = gui.lines;
    auto column = gui.column;

    ///Stampo le istruzioni
    int startY = lines/2-10;
    printXY(column/2+2, startY, "Posiziona le navi!");

    printXY(column/2+2, startY+2, "Utilizza le frecce direzionali per muoverti");
    printXY(column/2+2, startY+3, "lungo la griglia, 'Q' per ruotare la nave");
    printXY(column/2+2, startY+4, "e usa INVIO per confermare!");

    startY = startY+10;
    printXY(column/2+2, startY-1, "Hai a disposizione:");
    for(int i = 0; i < shipToPlaceSize.size(); i){
        int count = 0;
        int currentShipSize = shipToPlaceSize[i];
        while(shipToPlaceSize[i+count] == currentShipSize) {count++;}
        printXY(column/2+2, startY, to_string(count)+ "x navi di dimensione " + to_string(currentShipSize));

        startY++;
        i += count;
    }

    //Stampo il bordo
    auto outline = Box(0,0,column,lines);
    outline.setTitle(username);
    outline.setAttrColor(Color::Dim);
    outline.draw();

    // Posiziono dunque le navi
    for(int i = 0; i < shipToPlaceSize.size(); i++) {

        int size = board.grid.getSize();
        int _x = size / 2;              //Posizione iniziale del cursore e della nave
        int _y = _x;
        auto ship = Ship(board.getXforCell(_x), board.getYforCell(_y), shipToPlaceSize[i], vertical);
        ship.setAttrColor(Color::Bright);
        bool placed = false;

        //Fin chè non posiziono la nave corrente...
        while(!placed){
            board.draw();                        //Disegno la board

            ship.setX(board.getXforCell(_x));   //Aggiorno le coordinate della nave che sto posizionando
            ship.setY(board.getYforCell(_y));
            ship.draw();                        //Disegno la nave

            // Attendo l'input dell'utente
            switch (getch()) {
                case LEFT_ARROW:
                    if (_x > 0) _x--;  break;

                case UP_ARROW:
                    if (_y > 0) _y--;  break;

                case RIGHT_ARROW:
                    if (ship.isHorizontal() and _x < size - ship.getSize()) {
                        _x++;
                        break;
                    }
                    if (!ship.isHorizontal() and _x < size - 1) _x++;
                    break;

                case DOWM_ARROW:
                    if (!ship.isHorizontal() and _y < size - ship.getSize()) {
                        _y++;
                        break;
                    }
                    if (ship.isHorizontal() and _y < size - 1) _y++;
                    break;

                case 'q': case 'Q':
                    if (!ship.isHorizontal() and _x < size - ship.getSize() + 1) {
                        ship.rotate();
                        break;
                    }
                    if (ship.isHorizontal() and _y < size - ship.getSize() + 1) ship.rotate();
                    break;

                case ENTER:
                    ship.setAttrColor(Color::Reset);
                    if(board.addShip(_x, _y, ship)) placed = true;                              //Posiziono la nave
                    ship.setAttrColor(Color::Bright);

                default:
                    break;
            }
        }

    } //Fine for

    board.draw();
}


void statsScene(){
    gui.clearScreen();

    cout << " __" << endl;
    cout << "(_ _|_  _. _|_ o  _ _|_ o  _ |_   _     _| o    _  o  _   _  _ " << endl;
    cout << "__) |_ (_|  |_ | _>  |_ | (_ | | (/_   (_| |   (_| | (_) (_ (_)" << endl;
    cout << "                                                _|             " << endl;
    cout << "[1] Lista dei giocatori registrati;" << endl;
    cout << "[2] Storico delle partite giocate e terminate;" << endl;
    cout << "[3] Lista delle partite giocate in corso;" << endl;
    cout << "[9] Esci dalle satistiche e vai al gioco." << endl;
    cout << endl;
    cout << "Premi 1,2,3 o 9 per effettuare una scelta..." << endl << endl;

    while (true){
        switch (getch()){
            case '1': mc.registeredUsersRequest(); break;
            case '2': mc.terminatedMatchesRequest(); break;
            case '3': mc.inProgressMatchesRequest(); break;
            case '9': return;
            default:
                continue;
        }
        cout << endl << endl;
        cout << "Premi 1,2,3 o 9 per effettuare una scelta..." << endl;

    }
}