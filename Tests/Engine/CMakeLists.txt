cmake_minimum_required(VERSION 3.1)
project(BerserkTestsEngine VERSION 1.0)

add_executable(TestOpenGL TestOpenGL.cpp)
target_link_libraries(TestOpenGL PRIVATE BerserkCore)
target_link_libraries(TestOpenGL PRIVATE BerserkPlatform)
target_link_libraries(TestOpenGL PRIVATE OpenGL)