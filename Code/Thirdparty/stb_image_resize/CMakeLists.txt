cmake_minimum_required(VERSION 3.11)
project(StbImageResize VERSION 1.0 LANGUAGES CXX)

set(Target_SOURCES
    stb_image_resize.cpp
    stb_image_resize.h)

add_library(StbImageResize STATIC ${Target_SOURCES})

target_include_directories(StbImageResize PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(StbImageResize PUBLIC cxx_std_11)

set_target_properties(StbImageResize PROPERTIES CXX_STANDARD 11)
set_target_properties(StbImageResize PROPERTIES CXX_STANDARD_REQUIRED YES)