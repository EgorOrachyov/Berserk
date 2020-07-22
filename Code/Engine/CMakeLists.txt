cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        Resources/Mesh.h
        Resources/Texture.h
        Resources/Font.h

        )

set(Target_SOURCES
        Resources/Mesh.cpp
        Resources/Texture.cpp
        Resources/Font.cpp

        )

add_subdirectory(Rendering)

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_link_libraries(BerserkEngine PUBLIC BerserkRendering)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)

set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD_REQUIRED YES)