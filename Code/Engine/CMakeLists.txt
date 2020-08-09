cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        Debug/Console.h
        Input/TextInput.h
        )

set(Target_SOURCES
        Debug/Console.cpp
        Input/TextInput.cpp
        )

add_subdirectory(Render)
add_subdirectory(Resources)

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_link_libraries(BerserkEngine PUBLIC BerserkRender)
target_link_libraries(BerserkEngine PUBLIC BerserkResources)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)