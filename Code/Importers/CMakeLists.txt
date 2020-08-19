cmake_minimum_required(VERSION 3.11)
project(BerserkImporters VERSION 1.0 LANGUAGES CXX)

file(GLOB_RECURSE Target_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*.cpp")

add_library(BerserkImporters SHARED ${Target_SOURCES})
target_include_directories(BerserkImporters PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkImporters PUBLIC BerserkCore)
target_link_libraries(BerserkImporters PUBLIC BerserkEngine)
target_link_libraries(BerserkImporters PRIVATE StbImage)
target_link_libraries(BerserkImporters PRIVATE freetype)
target_link_libraries(BerserkImporters PRIVATE TinyObjLoader)
target_compile_features(BerserkImporters PUBLIC cxx_std_11)

set_target_properties(BerserkImporters PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkImporters PROPERTIES CXX_STANDARD_REQUIRED YES)