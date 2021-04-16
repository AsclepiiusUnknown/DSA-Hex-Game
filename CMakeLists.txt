cmake_minimum_required(VERSION 3.17)
project(DSA_Hex_Game2)

set(CMAKE_CXX_STANDARD 14)

add_executable(DSA_Hex_Game2 main.cpp Cell.h Board/Board.h Players/Minimax/MinimaxPlayer.h)

SET(CMAKE_BUILD_TYPE Debug)