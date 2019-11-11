cmake_minimum_required(VERSION 3.16)
project(td_game_server)

#include headers
include_directories(${PROJECT_BINARY_DIR}/_lib/chain)
include_directories(${PROJECT_BINARY_DIR}/_lib/queue)
include_directories(${PROJECT_BINARY_DIR}/_lib/vector2)
include_directories(${PROJECT_BINARY_DIR}/_lib/path)
include_directories(${PROJECT_BINARY_DIR}/_lib/basic_lib)
include_directories(${PROJECT_BINARY_DIR}/_lib/checkpoint_list)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/networking)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/networking/actions)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/networking/events)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/ecs/components)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/ecs/systems)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/ecs)

#add source directories
add_subdirectory(td_game_server/source/ecs/systems)
add_subdirectory(td_game_server/source/ecs)
add_subdirectory(td_game_server/source/networking)
add_subdirectory(td_game_server/source)
#Set threading flag
SET(CMAKE_CXX_FLAGS -pthread)
#add executable
add_executable(debug td_game_server/source/main.cpp)
#link source libraries
target_link_libraries(debug PRIVATE td_game_server)
#set bin output directory
set_target_properties(debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
