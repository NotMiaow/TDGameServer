cmake_minimum_required(VERSION 3.16)
project(td_game_server)

add_library(td_game_server
    ${ECS_SOURCE}
    ${NETWORKING_SOURCE}
    gameServer.cpp
)
