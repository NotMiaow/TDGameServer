project(td_game_server)

#include headers
include_directories(${PROJECT_BINARY_DIR}/_lib/checkpoint_list/headers)
include_directories(${PROJECT_BINARY_DIR}/td_game_server/headers/ecs)

#add source directories
add_subdirectory(td_game_server/source/ecs)
#add executable
add_executable(debug td_game_server/source/main.cpp)
#link source libraries
target_link_libraries(debug PRIVATE tdgs)
#set bin output directory
set_target_properties(debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_bin)
