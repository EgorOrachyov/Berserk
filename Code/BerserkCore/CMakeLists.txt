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
        BerserkCore/Platform/Window.hpp
        BerserkCore/Platform/WindowManager.cpp
        BerserkCore/Platform/WindowManager.hpp
        BerserkCore/Debug/Debug.cpp
        BerserkCore/Debug/Debug.hpp
        BerserkCore/Misc/MemoryBuffer.hpp
        BerserkCore/Misc/Singleton.hpp
        BerserkCore/Misc/Crc32.cpp
        BerserkCore/Misc/Crc32.hpp
        BerserkCore/Misc/Contracts.hpp
        BerserkCore/Misc/PtrShared.hpp
        BerserkCore/Misc/DateTime.hpp
        BerserkCore/Misc/Event.hpp
        BerserkCore/Misc/Ref.hpp
        BerserkCore/Strings/StringUtils.hpp
        BerserkCore/Strings/String.cpp
        BerserkCore/Strings/String.hpp
        BerserkCore/Strings/StringBuilder.cpp
        BerserkCore/Strings/StringBuilder.hpp
        BerserkCore/Strings/Formatter.hpp
        BerserkCore/Containers/Array.hpp
        BerserkCore/Containers/ArrayFixed.hpp
        BerserkCore/Threading/Synchronization.hpp
        BerserkCore/IO/Log.cpp
        BerserkCore/IO/Log.hpp
        BerserkCore/IO/TextWriter.hpp
        BerserkCore/IO/FileTextWriter.cpp
        BerserkCore/IO/FileTextWriter.hpp
        BerserkCore/Math/Utils.cpp
        BerserkCore/Math/Utils.hpp
        BerserkCore/Math/TMatMxN.hpp
        BerserkCore/Math/TVecN.hpp
        BerserkCore/Math/TQuat.hpp
        BerserkCore/Math/TAabb.hpp
        BerserkCore/Math/TPlane.hpp
        BerserkCore/Math/TFrustum.hpp
        BerserkCore/Math/Random.hpp
        BerserkCore/Math/Utils2d.hpp
        BerserkCore/Math/Utils3d.hpp
        BerserkCore/Math/Transformf.hpp
        BerserkCore/Memory/PoolAllocator.hpp
        BerserkCore/Memory/PoolsAllocator.cpp
        BerserkCore/Memory/PoolsAllocator.hpp
        )

add_library(BerserkCore SHARED ${BerserkCore_SOURCES})
target_link_libraries(BerserkCore PUBLIC Yamc)
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