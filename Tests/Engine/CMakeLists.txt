cmake_minimum_required(VERSION 3.1)
project(BerserkTestsEngine VERSION 1.0)

add_executable(TestEngine TestEngine.cpp)
target_link_libraries(TestEngine PRIVATE BerserkCore)
target_link_libraries(TestEngine PRIVATE BerserkEngine)
target_link_libraries(TestEngine PRIVATE BerserkPlatform)
target_link_libraries(TestEngine PRIVATE BerserkImporters)
