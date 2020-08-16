cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
        Debug/Console.h
        Debug/ConsoleCommands.h
        Debug/ConsoleMessages.h
        Input/TextInput.h
        Resources/Font.h
        Resources/FontGlyph.h
        Resources/FontImportOptions.h
        Resources/Mesh.h
        Resources/MeshNode.h
        Resources/MeshAttribute.h
        Resources/MeshImportOptions.h
        )

set(Target_SOURCES
        Debug/Console.cpp
        Debug/ConsoleCommands.cpp
        Debug/ConsoleMessages.cpp
        Input/TextInput.cpp
        Resources/Font.cpp
        Resources/Mesh.cpp
        Resources/MeshAttribute.cpp
        )

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_link_libraries(BerserkEngine PUBLIC BerserkRender)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)