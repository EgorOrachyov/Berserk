cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
    Rendering/Shader.h
    Rendering/ShaderDefsMacro.h
    Rendering/VertexPolicy.h
    Rendering/RenderModule.h
    Resources/Mesh.h
        )

set(Target_SOURCES
    Rendering/Shader.cpp
    Rendering/ShaderDefsMacro.cpp
    Rendering/VertexPolicy.cpp
    Rendering/RenderModule.cpp
    Resources/Mesh.cpp
        )

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_compile_features(BerserkEngine PUBLIC cxx_std_11)

set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD_REQUIRED YES)