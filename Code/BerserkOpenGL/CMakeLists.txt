cmake_minimum_required(VERSION 3.11)
project(BerserkOpenGL VERSION 1.0 LANGUAGES CXX)

set(BerserkOpenGL_SOURCES
        BerserkOpenGL/GLDefs.hpp
        BerserkOpenGL/GLDeferredResources.cpp
        BerserkOpenGL/GLDeferredResources.hpp
        BerserkOpenGL/GLDriver.cpp
        BerserkOpenGL/GLDriver.hpp
        BerserkOpenGL/GLDevice.cpp
        BerserkOpenGL/GLDevice.hpp
        BerserkOpenGL/GLContext.cpp
        BerserkOpenGL/GLContext.hpp
        BerserkOpenGL/GLVertexBuffer.cpp
        BerserkOpenGL/GLVertexBuffer.hpp

        BerserkOpenGL/GLUniformBuffer.cpp BerserkOpenGL/GLUniformBuffer.hpp BerserkOpenGL/GLIndexBuffer.cpp BerserkOpenGL/GLIndexBuffer.hpp)

add_library(BerserkOpenGL SHARED ${BerserkOpenGL_SOURCES})
target_include_directories(BerserkOpenGL PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkOpenGL PUBLIC BerserkCore)
target_link_libraries(BerserkOpenGL PUBLIC BerserkRHI)
target_link_libraries(BerserkOpenGL PRIVATE glew_shared)

target_compile_features(BerserkOpenGL PUBLIC cxx_std_11)

set_target_properties(BerserkOpenGL PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkOpenGL PROPERTIES CXX_STANDARD_REQUIRED YES)