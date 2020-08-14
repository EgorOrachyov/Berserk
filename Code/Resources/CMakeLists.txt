cmake_minimum_required(VERSION 3.11)
project(BerserkResources VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        Font.h
        FontGlyph.h
        FontImportOptions.h
        )

set(Target_SOURCES
        Font.cpp
        )

add_library(BerserkResources SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkResources PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkResources PUBLIC BerserkCore)
target_compile_features(BerserkResources PUBLIC cxx_std_11)

set_target_properties(BerserkResources PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkResources PROPERTIES CXX_STANDARD_REQUIRED YES)