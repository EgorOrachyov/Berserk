cmake_minimum_required(VERSION 3.1)
project(BerserkTestsCore VERSION 1.0)

add_executable(TestCoreMinimals TestCoreMinimals.cpp)
target_link_libraries(TestCoreMinimals PRIVATE BerserkCore)

add_executable(TestStrings TestStrings.cpp)
target_link_libraries(TestStrings PRIVATE BerserkCore)
target_link_libraries(TestStrings PRIVATE BerserkPlatform)

add_executable(TestMath TestMath.cpp)
target_link_libraries(TestMath PRIVATE BerserkCore)

add_executable(TestContainers TestContainers.cpp)
target_link_libraries(TestContainers PRIVATE BerserkCore)
target_link_libraries(TestContainers PRIVATE BerserkPlatform)

add_executable(TestPlatform TestPlatform.cpp)
target_link_libraries(TestPlatform PRIVATE BerserkCore)
target_link_libraries(TestPlatform PRIVATE BerserkPlatform)

add_test(NAME TestPlatform COMMAND TestPlatform)
