//
// Created by giuseppe on 27/12/16.
//

#ifndef BATTLESHIPSERVER_MATCHLIST_H
#define BATTLESHIPSERVER_MATCHLIST_H
#include <iostream>
#include <list>

using namespace std;

class MatchList {
public:
    struct Match{
        string username0;                               //Sfidante
        string username1;                               //Sfidato
        string date;
        bool winner;                                    //0 ha vinto username0  - 1 ha vinto username1
    };

    bool insert(string username0, string username1);
    bool contein(string username1, string username2);
    bool terminateMatch(string username0, bool username0Win);

    const list<Match> &getInPogressMatch() const { return inPogressMatch; }
    const list<Match> &getTerminatedMatch() const { return terminatedMatch; }

private:
    list<Match> inPogressMatch;
    list<Match> terminatedMatch;
};


#endif //BATTLESHIPSERVER_MATCHLIST_H
