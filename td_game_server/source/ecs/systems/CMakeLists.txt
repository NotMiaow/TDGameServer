cmake_minimum_required(VERSION 3.16)
project(td_game_server)

set(SYSTEMS_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/timeSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/movementSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pathfindingSystem.cpp
    PARENT_SCOPE
)
