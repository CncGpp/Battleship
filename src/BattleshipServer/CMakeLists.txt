cmake_minimum_required(VERSION 3.3)
project(BattleshipServer)
include_directories(../library)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Server.cpp Server.h
        ../library/lib.h ../library/lib.cpp
        ../library/Wrapper.h ../library/Wrapper.cpp
        ../library/Package.h ../library/Package.cpp
        MainServer.cpp MainServer.h
        ../library/UsersList.cpp ../library/UsersList.h
        data/RegistredUsers.cpp data/RegistredUsers.h
        data/MatchList.cpp data/MatchList.h)
add_executable(BattleshipServer ${SOURCE_FILES})