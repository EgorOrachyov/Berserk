cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

add_subdirectory(Rendering)

add_library(BerserkEngine INTERFACE)

target_link_libraries(BerserkEngine INTERFACE BerserkCore)
target_link_libraries(BerserkEngine INTERFACE BerserkRendering)
target_compile_features(BerserkEngine INTERFACE cxx_std_11)