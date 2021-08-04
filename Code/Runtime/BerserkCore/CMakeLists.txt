cmake_minimum_required(VERSION 3.11)
project(BerserkCore LANGUAGES CXX)

set(BerserkCore_SOURCES
        BerserkCore/Typedefs.hpp
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
        BerserkCore/Platform/Path.cpp
        BerserkCore/Platform/Path.hpp
        BerserkCore/Platform/FileSystem.cpp
        BerserkCore/Platform/FileSystem.hpp
        BerserkCore/Platform/Window.hpp
        BerserkCore/Platform/WindowManager.cpp
        BerserkCore/Platform/WindowManager.hpp
        BerserkCore/Platform/Thread.hpp
        BerserkCore/Platform/ThreadManager.cpp
        BerserkCore/Platform/ThreadManager.hpp
        BerserkCore/Platform/Dialogs.cpp
        BerserkCore/Platform/Dialogs.hpp
        BerserkCore/Platform/DateTime.hpp
        BerserkCore/Platform/Synchronization.hpp
        BerserkCore/Debug/Debug.cpp
        BerserkCore/Debug/Debug.hpp
        BerserkCore/Platform/Crc32.cpp
        BerserkCore/Platform/Crc32.hpp
        BerserkCore/Templates/Stack.hpp
        BerserkCore/Templates/Singleton.hpp
        BerserkCore/Templates/Contracts.hpp
        BerserkCore/Templates/SimplePtr.hpp
        BerserkCore/Templates/SmartPointer.hpp
        BerserkCore/Templates/SmartPointerInternal.hpp
        BerserkCore/Templates/Event.hpp
        BerserkCore/Templates/RefCounted.hpp
        BerserkCore/Templates/Mask.hpp
        BerserkCore/Templates/Array.hpp
        BerserkCore/Templates/ArrayFixed.hpp
        BerserkCore/Templates/Queue.hpp
        BerserkCore/Templates/HashTable.hpp
        BerserkCore/Templates/Pair.hpp
        BerserkCore/Templates/MemoryBuffer.hpp
        BerserkCore/Templates/MessageQueue.hpp
        BerserkCore/Templates/CommandQueue.hpp
        BerserkCore/Templates/AsyncCommandQueue.hpp
        BerserkCore/Templates/AsyncCommandQueue.hpp
        BerserkCore/Templates/HashMap.hpp
        BerserkCore/Templates/HashSet.hpp
        BerserkCore/Templates/Timer.hpp
        BerserkCore/Strings/BasicString.cpp
        BerserkCore/Strings/BasicString.hpp
        BerserkCore/Strings/StringUtils.hpp
        BerserkCore/Strings/String.cpp
        BerserkCore/Strings/String.hpp
        BerserkCore/Strings/StringName.cpp
        BerserkCore/Strings/StringName.hpp
        BerserkCore/Strings/StringBuilder.cpp
        BerserkCore/Strings/StringBuilder.hpp
        BerserkCore/Strings/Formatter.hpp
        BerserkCore/Strings/Unicode.cpp
        BerserkCore/Strings/Unicode.hpp
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
        BerserkCore/Memory/LinearAllocator.hpp
        BerserkCore/Image/Color.hpp
        BerserkCore/Image/Color.cpp
        BerserkCore/Image/Image.cpp
        BerserkCore/Image/Image.hpp
        BerserkCore/Image/ImageLoadImpl.cpp
        BerserkCore/Image/ImageWriteImpl.cpp
        BerserkCore/Image/PixelUtil.cpp
        BerserkCore/Image/PixelUtil.hpp
        BerserkCore/Image/PixelData.hpp
        BerserkCore/Image/PixelData.cpp
        BerserkCore/Threading/Task.cpp
        BerserkCore/Threading/Task.hpp
        BerserkCore/Threading/TaskManager.cpp
        BerserkCore/Threading/TaskManager.hpp
        BerserkCore/Threading/TaskContext.hpp
        BerserkCore/Threading/AsyncOp.hpp
        BerserkCore/Threading/TaskQueue.cpp
        BerserkCore/Threading/TaskQueue.hpp
        )

add_library(BerserkCore STATIC ${BerserkCore_SOURCES})
target_link_libraries(BerserkCore PUBLIC Yamc)
target_link_libraries(BerserkCore PRIVATE StbImage)
target_link_libraries(BerserkCore PRIVATE portable_file_dialogs)
target_include_directories(BerserkCore PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_features(BerserkCore PUBLIC cxx_std_11)
target_compile_options(BerserkCore PUBLIC $<$<CXX_COMPILER_ID:MSVC>: /source-charset:utf-8>)

foreach(DEFINITION ${BERSERK_DEFINES})
    target_compile_definitions(BerserkCore PUBLIC ${DEFINITION})
endforeach()

set_target_properties(BerserkCore PROPERTIES CXX_STANDARD 11)
set_target_properties(BerserkCore PROPERTIES CXX_STANDARD_REQUIRED YES)
