cmake_minimum_required(VERSION 3.16)
project(td_game_server)

set(NETWORKING_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/actionManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/networkManager.cpp
    PARENT_SCOPE
)
