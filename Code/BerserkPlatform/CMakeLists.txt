cmake_minimum_required(VERSION 3.11)
project(BerserkPlatform VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE Target_HEADERS ${CMAKE_CURRENT_LIST_DIR} "BerserkPlatform/*.hpp")
file(GLOB_RECURSE Target_SOURCES ${CMAKE_CURRENT_LIST_DIR} "BerserkPlatform/*.cpp")

add_library(BerserkPlatform SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkPlatform PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkPlatform PUBLIC BerserkCore)
target_compile_features(BerserkPlatform PUBLIC cxx_std_11)

set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkPlatform PROPERTIES CXX_STANDARD_REQUIRED YES)