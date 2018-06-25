cmake_minimum_required(VERSION 3.9)
project(BerserkEngine)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -v")

add_executable(BerserkEngine
        Debug/main.cpp
        Core/Essential/Types.h
        Core/Essential/CompilationFlags.h
        Core/Essential/Assert.h
        Core/Essential/Common.h
        Sources/LogManager.cpp
        Core/Logging/LogManager.h
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
        Core/Essential/Timer.h
        Core/Essential/StandardTimer.h
        Core/Memory/MemoryAllocators.h
        Sources/MemoryAllocators.cpp
        Sources/MemoryProfiler.cpp
        Core/Memory/StaticPoolAllocator.h
        Core/Essential/UsageDescriptors.h
        Core/Profiling/Profiling.h
        Core/Essential/Cast.h
        Sources/StaticPoolAllocator.cpp
        Debug/ClassTesting.h
        Core/Containers/HashTable.h
        Sources/StringASCII.cpp
        Core/Strings/StringASCII.h
        Sources/StringUTF32.cpp
        Core/Strings/StringUTF32.h
        Sources/MemoryFactory.cpp
        Core/Memory/MemoryFactory.h
        Sources/ContainersFactory.cpp
        Core/Containers/ContainersFactory.h
        Sources/StringsFactory.cpp
        Core/Strings/StringsFactory.h
        Sources/StaticStringASCII.cpp
        Core/Strings/StaticStringASCII.h
        Core/Strings/StringUtils.h
        Core/Strings/StringFlags.h
        Sources/ApplicationContext.cpp
        Application/ApplicationContext.h
        Debug/TestLinkArgPassing.h
        Core/Logging/LogCompilationMacros.h
        Core/Logging/LogProperties.h
        Core/Maths/BasicArithmetic.h
        Sources/BasicArithmetic.cpp
        Sources/StaticStringUTF32.cpp
        Core/Strings/StaticStringUTF32.h)