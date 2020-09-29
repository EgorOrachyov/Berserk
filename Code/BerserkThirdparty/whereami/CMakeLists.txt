cmake_minimum_required(VERSION 3.11)
project(WhereAmI)

add_library(WhereAmI src/whereami.h src/whereami.c)
target_include_directories(WhereAmI PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")