cmake_minimum_required(VERSION 3.11)
project(OpenGL)

add_library(OpenGL STATIC include/glad/glad.h src/glad.c)
target_include_directories(OpenGL PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")