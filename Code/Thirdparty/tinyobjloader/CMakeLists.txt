cmake_minimum_required(VERSION 3.11)
project(TinyObjLoader VERSION 1.0 LANGUAGES CXX)

set(Target_SOURCES
        tiny_obj_loader.cpp
        tiny_obj_loader.h
        )

add_library(TinyObjLoader STATIC ${Target_SOURCES})

target_include_directories(TinyObjLoader PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(TinyObjLoader PUBLIC cxx_std_11)

set_target_properties(TinyObjLoader PROPERTIES CXX_STANDARD 11)
set_target_properties(TinyObjLoader PROPERTIES CXX_STANDARD_REQUIRED YES)