cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE Target_HEADERS ${CMAKE_CURRENT_LIST_DIR} "BerserkCore/*.hpp")
file(GLOB_RECURSE Target_SOURCES ${CMAKE_CURRENT_LIST_DIR} "BerserkCore/*.cpp")

add_library(BerserkCore SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_features(BerserkCore PUBLIC cxx_std_11)

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)