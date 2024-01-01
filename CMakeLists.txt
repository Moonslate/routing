#Require a minimum version
cmake_minimum_required(VERSION 3.10)

project(uva-routing)

add_library(uva-routing
	${CMAKE_CURRENT_LIST_DIR}/src/routing.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/parameters.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/basic_before_action.cpp
)

include_directories(${CMAKE_CURRENT_LIST_DIR})

#Tests

#Create tests for parent project
if(TARGET cspec)
	cspec_configure("uva-routing" ${CMAKE_CURRENT_LIST_DIR} "uva-routing")
endif()