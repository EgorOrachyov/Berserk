cmake_minimum_required(VERSION 3.9)
project(BerserkEngine)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v")

add_executable(BerserkEngine
        Debug/main.cpp
        Core/Types.h
        Core/CompilationFlags.h
        Core/Assert.h
        Core/Common.h
        Sources/LogManager.cpp
        Core/LogManager.h
        Core/Containers/LinkedList.h
        Sources/ArrayList.cpp
        Core/Containers/ArrayList.h
        Sources/DoublyLinkedList.cpp
        Core/Containers/DoublyLinkedList.h
        Sources/UnrolledList.cpp
        Core/Containers/UnrolledList.h
        Core/Profiling/MemoryProfiler.h
        Sources/StackAllocator.cpp
        Core/Memory/StackAllocator.h
        Sources/DoubleStackAllocator.cpp
        Core/Memory/DoubleStackAllocator.h
        Sources/DoubleFrameAllocator.cpp
        Core/Memory/DoubleFrameAllocator.h
        Sources/ResourceManager.cpp
        Core/ResourceManager.h
        Core/Timer.h
        Core/StandardTimer.h
        Core/MemoryAllocators.h
        Sources/MemoryAllocators.cpp
        Sources/MemoryProfiler.cpp
        Core/Memory/PoolAllocator.h
        Core/UsageDescriptors.h
        Core/Profiling/Profiling.h
        Core/Cast.h
        Sources/PoolAllocator.cpp
        Debug/ClassTesting.h Core/Containers/HashTable.h Sources/CString.cpp Core/Strings/CString.h Sources/WString.cpp Core/Strings/WString.h Sources/MemoryFactory.cpp Core/Memory/MemoryFactory.h Sources/ContainersFactory.cpp Core/Containers/ContainersFactory.h Sources/StringsFactory.cpp Core/Strings/StringsFactory.h)