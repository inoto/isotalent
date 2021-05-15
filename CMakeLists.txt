cmake_minimum_required(VERSION 3.19)
project(src)

set(CMAKE_CXX_STANDARD 14)

#set(PROJECT_NAME isotalent)
#set(SOURCE_FILES main.cpp)

include_directories(src)

add_executable(src
        src/main.cpp
        src/Map.cpp
        src/Map.h
        src/Pathfinder.cpp
        src/Pathfinder.h)

# SFML
set(SFML_DIR "${PROJECT_SOURCE_DIR}/../SFML-2.5.1")
set(SFML_ROOT "${SFML_DIR}")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

message ("PROJECT_NAME: ${PROJECT_NAME}")
message ("SFML_INCLUDE_DIR: ${SFML_ROOT}/include")
message ("SFML_LIBRARIES: ${SFML_ROOT}/lib")

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message("included: ${dir}")
endforeach()
