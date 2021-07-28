set(BerserkTestUtils_SOURCES
        PlatformSetup.hpp
        PlatformSetup.cpp)

add_library(BerserkTestUtils STATIC ${BerserkTestUtils_SOURCES})
target_include_directories(BerserkTestUtils PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(BerserkTestUtils PUBLIC gtest)
target_link_libraries(BerserkTestUtils PUBLIC BerserkCore)
target_link_libraries(BerserkTestUtils PUBLIC BerserkPlatform)

