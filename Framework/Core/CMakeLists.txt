cmake_minimum_required(VERSION 3.9)
project(BerserkCore)

# Core Subsystem files

set(SOURCE_LIB
        Essential/Types.h
        Essential/CompilationFlags.h
        Essential/Assert.h
        Essential/Common.h
        Sources/LogManager.cpp
        Logging/LogManager.h
        Containers/LinkedList.h
        Containers/ArrayList.h
        Containers/DoubleLinkedList.h
        Containers/UnrolledList.h
        Profiling/MemoryProfiler.h
        Sources/StackAllocator.cpp
        Memory/StackAllocator.h
        Sources/DoubleStackAllocator.cpp
        Memory/DoubleStackAllocator.h
        Sources/DoubleFrameAllocator.cpp
        Memory/DoubleFrameAllocator.h
        Essential/Timer.h
        Essential/StandardTimer.h
        Memory/MemoryAllocators.h
        Sources/MemoryAllocators.cpp
        Sources/MemoryProfiler.cpp
        Memory/PoolAllocator.h
        Essential/UsageDescriptors.h
        Profiling/Profiling.h
        Essential/Cast.h
        Sources/PoolAllocator.cpp
        Containers/HashTable.h
        Sources/CHARString.cpp
        Strings/CHARString.h
        Sources/WCAHRString.cpp
        Strings/WCAHRString.h
        Sources/MemoryFactory.cpp
        Memory/MemoryFactory.h
        Sources/ContainersFactory.cpp
        Containers/ContainersFactory.h
        Sources/StringsFactory.cpp
        Strings/StringsFactory.h
        Sources/CHARStaticString.cpp
        Strings/CHARStaticString.h
        Strings/StringUtils.h
        Strings/StringFlags.h
        Logging/LogMessages.h
        Logging/LogProperties.h
        Math/UtilityNumbers.h
        Sources/UtilityNumbers.cpp
        Sources/WCAHRStaticString.cpp
        Strings/WCAHRStaticString.h
        Sources/Buffers.cpp
        Essential/Buffers.h
        Sources/Vector2.cpp
        Math/Vector2.h
        Sources/Vector3.cpp
        Math/Vector3.h
        Sources/Vector4.cpp
        Math/Vector4.h
        Sources/Matrix2x2.cpp
        Math/Matrix2x2.h
        Sources/Matrix3x3.cpp
        Math/Matrix3x3.h
        Sources/Matrix4x4.cpp
        Math/Matrix4x4.h
        Math/UtilityVectors.h
        Sources/UtilityVectors.cpp
        Math/UtilityMatrices.h
        Sources/UtilityMatrices.cpp
        Sources/Quaternion.cpp
        Math/Quaternion.h
        Math/UtilityQuaternions.h
        Sources/UtilityQuaternions.cpp
        HashFunctions/CRC32.h
        Sources/CRC32.cpp)

# Create static Core Subsystem library

add_library(BerserkCore STATIC ${SOURCE_LIB})