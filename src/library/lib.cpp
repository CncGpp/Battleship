//
// Created by giuseppe on 17/12/16.
//

#include "lib.h"

const std::string currentTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "[%X]:\t", &tstruct);

    return buf;
}


const std::string currentDate() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%d-%m-%Y %X", &tstruct);

    return buf;
}