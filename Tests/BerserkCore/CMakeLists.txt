add_executable(TestStrings TestStrings.cpp)
target_link_libraries(TestStrings PUBLIC BerserkCore)
target_link_libraries(TestStrings PUBLIC BerserkTestUtils)

add_executable(TestArray TestArray.cpp)
target_link_libraries(TestArray PUBLIC BerserkCore)
target_link_libraries(TestArray PUBLIC BerserkTestUtils)

add_executable(TestArrayFixed TestArrayFixed.cpp)
target_link_libraries(TestArrayFixed PUBLIC BerserkCore)
target_link_libraries(TestArrayFixed PUBLIC BerserkTestUtils)

add_executable(TestQueue TestQueue.cpp)
target_link_libraries(TestQueue PUBLIC BerserkCore)
target_link_libraries(TestQueue PUBLIC BerserkTestUtils)

add_executable(TestFileSystem TestFileSystem.cpp)
target_link_libraries(TestFileSystem PUBLIC BerserkCore)
target_link_libraries(TestFileSystem PUBLIC BerserkTestUtils)

add_executable(TestSystem TestSystem.cpp)
target_link_libraries(TestSystem PUBLIC BerserkCore)
target_link_libraries(TestSystem PUBLIC BerserkTestUtils)

add_executable(TestMath TestMath.cpp)
target_link_libraries(TestMath PUBLIC BerserkCore)
target_link_libraries(TestMath PUBLIC BerserkTestUtils)

add_executable(TestMemory TestMemory.cpp)
target_link_libraries(TestMemory PUBLIC BerserkCore)
target_link_libraries(TestMemory PUBLIC BerserkTestUtils)