cmake_minimum_required(VERSION 3.11)
project(BerserkUnix VERSION 1.0 LANGUAGES CXX)

set(BerserkLinux_SOURCES
        BerserkUnix/UnixSystem.cpp
        BerserkUnix/UnixSystem.hpp
        BerserkUnix/UnixEntryPoint.cpp
        BerserkUnix/UnixFileSystem.cpp
        BerserkUnix/UnixFileSystem.hpp
        BerserkUnix/UnixFile.cpp
        BerserkUnix/UnixFile.hpp
        BerserkUnix/UnixConsole.cpp
        BerserkUnix/UnixConsole.hpp
        BerserkUnix/UnixStringTable.hpp
        BerserkUnix/UnixThread.cpp
        BerserkUnix/UnixThread.hpp
        BerserkUnix/UnixThreadManager.cpp
        BerserkUnix/UnixThreadManager.hpp
        )

add_library(BerserkUnix SHARED ${BerserkLinux_SOURCES})
target_include_directories(BerserkUnix PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BerserkUnix PUBLIC BerserkCore)
target_link_libraries(BerserkUnix PRIVATE BerserkGlfw)
target_link_libraries(BerserkUnix PRIVATE WhereAmI)

if (BERSERK_WITH_OPENGL)
    target_link_libraries(BerserkUnix PRIVATE BerserkOpenGL)
endif()

target_compile_features(BerserkUnix PUBLIC cxx_std_11)

set_target_properties(BerserkUnix PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkUnix PROPERTIES CXX_STANDARD_REQUIRED YES)