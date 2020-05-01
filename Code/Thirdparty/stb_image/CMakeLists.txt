cmake_minimum_required(VERSION 3.11)
project(StbImage VERSION 1.0 LANGUAGES CXX)

set(Target_SOURCES
    stb_image.cpp
    stb_image.h
        )

add_library(StbImage STATIC ${Target_SOURCES})

target_include_directories(StbImage PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(StbImage PUBLIC cxx_std_11)

set_target_properties(StbImage PROPERTIES CXX_STANDARD 11)
set_target_properties(StbImage PROPERTIES CXX_STANDARD_REQUIRED YES)