cmake_minimum_required(VERSION 3.11)
project(BerserkMain VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE Target_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.cpp")

add_library(BerserkMain SHARED ${Target_SOURCES})
target_include_directories(BerserkMain PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkMain PUBLIC BerserkCore)
target_link_libraries(BerserkMain PUBLIC BerserkEngine)
target_link_libraries(BerserkMain PUBLIC BerserkPlatform)
target_link_libraries(BerserkMain PUBLIC BerserkImporters)

target_compile_features(BerserkMain INTERFACE cxx_std_11)