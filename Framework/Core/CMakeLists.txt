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
        Sources/StringASCII.cpp
        Strings/StringASCII.h
        Sources/StringUTF32.cpp
        Strings/StringUTF32.h
        Sources/MemoryFactory.cpp
        Memory/MemoryFactory.h
        Sources/ContainersFactory.cpp
        Containers/ContainersFactory.h
        Sources/StringsFactory.cpp
        Strings/StringsFactory.h
        Sources/StaticStringASCII.cpp
        Strings/StaticStringASCII.h
        Strings/StringUtils.h
        Strings/StringFlags.h
        Logging/LogMessages.h
        Logging/LogProperties.h
        Maths/UtilityNumbers.h
        Sources/UtilityNumbers.cpp
        Sources/StaticStringUTF32.cpp
        Strings/StaticStringUTF32.h
        Sources/Buffers.cpp
        Essential/Buffers.h
        Sources/Vector2.cpp
        Maths/Vector2.h
        Sources/Vector3.cpp
        Maths/Vector3.h
        Sources/Vector4.cpp
        Maths/Vector4.h
        Sources/Matrix2x2.cpp
        Maths/Matrix2x2.h
        Sources/Matrix3x3.cpp
        Maths/Matrix3x3.h
        Sources/Matrix4x4.cpp
        Maths/Matrix4x4.h
        Maths/UtilityVectors.h
        Sources/UtilityVectors.cpp
        Maths/UtilityMatrices.h
        Sources/UtilityMatrices.cpp
        Sources/Quaternion.cpp
        Maths/Quaternion.h
        Maths/UtilityQuaternions.h
        Sources/UtilityQuaternions.cpp
        HashFunctions/CRC32.h
        Sources/CRC32.cpp)

# Create static Core Subsystem library

add_library(BerserkCore STATIC ${SOURCE_LIB})