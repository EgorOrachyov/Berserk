cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

set(BerserkCore_SOURCES
        BerserkCore/Typedefs.hpp
        BerserkCore/Assert.hpp
        BerserkCore/Defines.hpp
        BerserkCore/Error/Exception.hpp
        BerserkCore/Platform/Atomic.hpp
        BerserkCore/Platform/Memory.cpp
        BerserkCore/Platform/Memory.hpp
        BerserkCore/Platform/Allocator.hpp
        BerserkCore/Platform/Allocator.cpp
        BerserkCore/Platform/System.cpp
        BerserkCore/Platform/System.hpp
        BerserkCore/Platform/EntryPoint.hpp
        BerserkCore/Platform/File.hpp
        BerserkCore/Platform/FileSystem.cpp
        BerserkCore/Platform/FileSystem.hpp
        BerserkCore/Misc/MemoryBuffer.hpp
        BerserkCore/Misc/Singleton.hpp
        BerserkCore/Misc/Crc32.cpp
        BerserkCore/Misc/Crc32.hpp
        BerserkCore/Misc/Contracts.hpp
        BerserkCore/Misc/PtrShared.hpp
        BerserkCore/Misc/DateTime.cpp
        BerserkCore/Misc/DateTime.hpp
        BerserkCore/Strings/StringUtils.hpp
        BerserkCore/Strings/String.cpp
        BerserkCore/Strings/String.hpp
        BerserkCore/Strings/StringBuilder.cpp
        BerserkCore/Strings/StringBuilder.hpp
        BerserkCore/Strings/Format.hpp
        BerserkCore/Containers/Array.hpp
        BerserkCore/Debug/Log.cpp
        BerserkCore/Debug/Log.hpp

        )

add_library(BerserkCore SHARED ${BerserkCore_SOURCES})
target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(BerserkCore PUBLIC cxx_std_11)

if(BERSERK_DEBUG)
    target_compile_definitions(BerserkCore PUBLIC _BERSERK_DEBUG)
endif()

if (BERSERK_RELEASE)
    target_compile_definitions(BerserkCore PUBLIC _BERSERK_RELEASE)
endif()

if (BERSERK_EDITOR)
    target_compile_definitions(BerserkCore PUBLIC _BERSERK_EDITOR)
endif()

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)