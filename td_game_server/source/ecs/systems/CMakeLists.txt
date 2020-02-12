cmake_minimum_required(VERSION 3.16)
project(td_game_server)

set(SYSTEMS_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/timeSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pathfindingSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rageSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/movementSystem.cpp
    PARENT_SCOPE
)
