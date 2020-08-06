cmake_minimum_required(VERSION 3.11)
project(BerserkEngine VERSION 1.0 LANGUAGES CXX)

add_subdirectory(Render)
add_subdirectory(Resources)

add_library(BerserkEngine INTERFACE)

target_link_libraries(BerserkEngine INTERFACE BerserkCore)
target_link_libraries(BerserkEngine INTERFACE BerserkRender)
target_link_libraries(BerserkEngine INTERFACE BerserkResources)
target_compile_features(BerserkEngine INTERFACE cxx_std_11)