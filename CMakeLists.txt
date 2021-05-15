cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME isotalent)

project(${PROJECT_NAME})
message ("PROJECT_NAME: ${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 14)

#set(PROJECT_NAME isotalent)
#set(SOURCE_FILES main.cpp)

include_directories(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Map.cpp
        src/Map.h
        src/Pathfinder.cpp
        src/Pathfinder.h)

# SFML
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_ROOT "${PROJECT_SOURCE_DIR}/../SFML-2.5.1")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

find_package(SFML 2 REQUIRED system window graphics)

#include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window)

file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
