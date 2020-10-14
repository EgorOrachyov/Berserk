add_library(TestDeps INTERFACE)
target_include_directories(TestDeps INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(TestDeps INTERFACE gtest)
target_link_libraries(TestDeps INTERFACE BerserkCore)
target_link_libraries(TestDeps INTERFACE BerserkPlatform)

add_executable(TestContainers TestContainers.cpp)
target_link_libraries(TestContainers PUBLIC TestDeps)

add_executable(TestStrings TestStrings.cpp)
target_link_libraries(TestStrings PUBLIC TestDeps)

add_executable(TestEssential TestEssential.cpp)
target_link_libraries(TestEssential PUBLIC TestDeps)

if (BERSERK_WITH_GLFW)
    add_executable(TestGlfw TestGlfw.cpp)
    target_link_libraries(TestGlfw PUBLIC TestDeps)
    target_link_libraries(TestGlfw PUBLIC glfw)
endif()