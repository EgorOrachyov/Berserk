cmake_minimum_required(VERSION 3.11)
project(BerserkCore VERSION 1.0 LANGUAGES CXX)

set(BerserkCore_SOURCES
        BerserkCore/Typedefs.hpp
        BerserkCore/Assert.hpp
        BerserkCore/Error/Exception.hpp
        BerserkCore/Platform/Memory.cpp
        BerserkCore/Platform/Memory.hpp
        BerserkCore/Platform/Allocator.hpp
        BerserkCore/Platform/Allocator.cpp
        BerserkCore/Platform/System.cpp
        BerserkCore/Platform/System.hpp
        BerserkCore/Misc/MemoryBuffer.hpp
        BerserkCore/Misc/Singleton.hpp
        BerserkCore/String/StringUtils.hpp
        BerserkCore/String/String.cpp
        BerserkCore/String/String.hpp
        )

add_library(BerserkCore SHARED ${BerserkCore_SOURCES})
target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(BerserkCore PUBLIC ${CMAKE_BINARY_DIR}/Code/BerserkCore)
target_compile_features(BerserkCore PUBLIC cxx_std_11)

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)