cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE Target_HEADERS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.h")
file(GLOB_RECURSE Target_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.cpp")

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_link_libraries(BerserkEngine PUBLIC BerserkRender)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)