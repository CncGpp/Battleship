//
// Created by giuseppe on 27/12/16.
//

#include <lib.h>
#include "MatchList.h"

bool MatchList::contein(string username0, string username1) {
    for(auto x : inPogressMatch)
        if(x.username0 == username0 and x.username1 == username1) return true;
    return false;
}

bool MatchList::insert(string username0, string username1) {
    if(contein(username0, username1)) return false;
    const struct Match temp = {username0, username1, currentDate(), false};
    inPogressMatch.push_front(temp);
    return true;
}

bool MatchList::terminateMatch(string username0, bool username0Win) {         //Basta solo 1 perchè è univoco

    auto it = inPogressMatch.begin();
    for (it; it != inPogressMatch.end(); it++)
        if (it->username0 == username0) {
            it->winner = !username0Win;
            terminatedMatch.push_back(*it);
            inPogressMatch.erase(it);
            return true;
        } else if (it->username1 == username0) {
            it->winner = username0Win;
            terminatedMatch.push_back(*it);
            inPogressMatch.erase(it);
            return true;
        }

    return false;
}