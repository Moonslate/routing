#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(routing)

add_executable(routing ${CMAKE_CURRENT_LIST_DIR}/src/routing_tool.cpp)

target_link_libraries(routing uva-routing)

add_custom_command(TARGET routing 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:routing> ${CMAKE_SOURCE_DIR}/bin)