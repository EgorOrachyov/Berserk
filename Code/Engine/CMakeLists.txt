cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

set(Target_HEADERS
    Resources/Image.h
    RHI/RHIDevice.h
    RHI/RHIResources.h
    RHI/RHIDefinitions.h)

set(Target_SOURCES
    Resources/Image.cpp)

add_library(BerserkEngine SHARED ${Target_HEADERS} ${Target_SOURCES})

target_link_libraries(BerserkEngine PUBLIC BerserkCore)
target_include_directories(BerserkEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(BerserkEngine PUBLIC cxx_std_11)

set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkEngine PROPERTIES CXX_STANDARD_REQUIRED YES)