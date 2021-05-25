cmake_minimum_required(VERSION 3.11)
project(BerserkGlfw LANGUAGES CXX)

set(BerserkGlfw_SOURCES
        BerserkGlfw/GlfwContext.cpp
        BerserkGlfw/GlfwContext.hpp
        BerserkGlfw/GlfwWindow.cpp
        BerserkGlfw/GlfwWindow.hpp
        BerserkGlfw/GlfwWindowManager.cpp
        BerserkGlfw/GlfwWindowManager.hpp
        )

add_library(BerserkGlfw STATIC ${BerserkGlfw_SOURCES})
target_include_directories(BerserkGlfw PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkGlfw PUBLIC BerserkCore)
target_link_libraries(BerserkGlfw PUBLIC glfw)

if (BERSERK_WITH_VULKAN)
    target_link_libraries(BerserkGlfw PRIVATE Vulkan)
    target_compile_definitions(BerserkGlfw PRIVATE BERSERK_WITH_VULKAN)
endif()

target_compile_features(BerserkGlfw PUBLIC cxx_std_11)

set_target_properties(BerserkGlfw PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkGlfw PROPERTIES CXX_STANDARD_REQUIRED YES)