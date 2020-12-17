cmake_minimum_required(VERSION 3.11)
project(BerserkLinux VERSION 1.0 LANGUAGES CXX)

set(BerserkLinux_SOURCES
        BerserkLinux/LinuxSystem.cpp
        BerserkLinux/LinuxSystem.hpp
        BerserkLinux/LinuxEntryPoint.cpp)

add_library(BerserkLinux SHARED ${BerserkLinux_SOURCES})
target_include_directories(BerserkLinux PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkLinux PUBLIC BerserkCore)

target_compile_features(BerserkLinux PUBLIC cxx_std_11)

set_target_properties(BerserkLinux PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkLinux PROPERTIES CXX_STANDARD_REQUIRED YES)