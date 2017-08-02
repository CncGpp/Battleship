cmake_minimum_required(VERSION 3.3)
project(BattleshipClient)
include_directories(../library)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp Client.cpp Client.h ../library/Wrapper.h ../library/Wrapper.cpp MainClient.cpp MainClient.h
        ../library/Package.h ../library/Package.cpp
        ../library/UsersList.h ../library/UsersList.cpp
        GUI/GUI.cpp GUI/GUI.h GUI/Terminal.cpp GUI/Terminal.h GUI/Color.cpp GUI/Color.h
        GUI/Graphics/Box.h GUI/Graphics/Box.cpp
        GUI/Graphics/TextBox.h GUI/Graphics/TextBox.cpp
        GUI/Graphics/Ship.h GUI/Graphics/Ship.cpp
        GUI/Graphics/Grid.h GUI/Graphics/Grid.cpp
        GUI/Graphics/Drawable.h GUI/Graphics/Drawable.cpp
        GUI/Graphics/Mark.h GUI/Graphics/Mark.cpp
        SubClient.cpp SubClient.h SubServer.cpp SubServer.h GUI/Board.cpp GUI/Board.h)
add_executable(BattleshipClient ${SOURCE_FILES})