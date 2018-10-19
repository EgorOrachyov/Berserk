cmake_minimum_required(VERSION 3.9)
project(BerserkCore)

# Core Subsystem files

set(SOURCE_LIB
        Misc/Types.h
        Misc/Compilation.h
        Misc/Assert.h
        Source/LogManager.cpp
        Logging/LogManager.h
        Containers/SharedList.h
        Containers/ArrayList.h
        Containers/DoubleLinkedList.h
        Containers/UnrolledList.h
        Source/StackAllocator.cpp
        Memory/StackAllocator.h
        Source/DoubleStackAllocator.cpp
        Memory/DoubleStackAllocator.h
        Source/DoubleFrameAllocator.cpp
        Memory/DoubleFrameAllocator.h
        Misc/Timer.h
        Misc/StandardTimer.h
        Memory/MemoryAllocators.h
        Source/MemoryAllocators.cpp
        Memory/PoolAllocator.h
        Misc/UsageDescriptors.h
        Profiling/Profiling.h
        Misc/Cast.h
        Source/PoolAllocator.cpp
        Containers/HashTable.h
        Source/CString.cpp
        Strings/CString.h
        Source/WString.cpp
        Strings/WString.h
        Source/CStaticString.cpp
        Strings/CStaticString.h
        Strings/UtilityString.h
        Strings/StringFlags.h
        Logging/LogMessages.h
        Logging/LogProperties.h
        Math/MathUtility.h
        Source/MathUtility.cpp
        Source/WStaticString.cpp
        Strings/WStaticString.h
        Misc/Buffers.h
        Source/Vector2f.cpp
        Math/Vector2f.h
        Source/Vector3f.cpp
        Math/Vector3f.h
        Source/Vector4f.cpp
        Math/Vector4f.h
        Source/Matrix2x2f.cpp
        Math/Matrix2x2f.h
        Source/Matrix3x3f.cpp
        Math/Matrix3x3f.h
        Source/Matrix4x4f.cpp
        Math/Matrix4x4f.h
        Math/UtilityMatrices.h
        Source/UtilityMatrices.cpp
        Source/Quatf.cpp
        Math/Quatf.h
        Math/UtilityQuaternions.h
        Source/UtilityQuaternions.cpp
        HashFunctions/CRC32.h
        Source/CRC32.cpp
        Containers/LinkedList.h
        Source/StringTable.cpp
        Strings/StringTable.h
        Source/CStringBuffer.cpp
        Strings/CStringBuffer.h
        Source/WStringBuffer.cpp
        Strings/WStringBuffer.h
        Source/ConfigTable.cpp
        Config/ConfigTable.h
        Source/ConfigLoader.cpp
        Config/ConfigLoader.h
        Config/ConfigCommon.h
        Containers/List.h
        Memory/ListAllocator.h Math/MathInclude.h Misc/Platform.h Source/Buffers.cpp Misc/Delete.h Misc/Include.h Misc/Build.h)

# Create static Core Subsystem library

add_library(BerserkCore STATIC ${SOURCE_LIB})