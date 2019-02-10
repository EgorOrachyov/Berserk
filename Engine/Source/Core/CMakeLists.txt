cmake_minimum_required(VERSION 3.9)
project(BerserkCore)

# Core Subsystem files

set(SOURCE_LIB

        # Misc submodule's files

        Private/Misc/Buffers.cpp
        Private/Misc/Crc32.cpp
        Public/Misc/Assert.h
        Public/Misc/Buffers.h
        Public/Misc/Types.h
        Public/Misc/Platform.h
        Public/Misc/Alignment.h
        Public/Misc/Include.h
        Public/Misc/Delete.h
        Public/Misc/UsageDescriptors.h
        Public/Misc/SIMD.h
        Public/Misc/Cast.h
        Public/Misc/Crc32.h
        Public/Misc/Inline.h
        Public/Misc/Compilation.h

        # Logging submodule's files

        Private/Logging/LogManager.cpp
        Public/Logging/LogVerbosity.h
        Public/Logging/LogMacros.h
        Public/Logging/LogManager.h

        # Memory submodule's files

        Private/Memory/PoolAllocator.cpp
        Private/Memory/StackAllocator.cpp
        Private/Memory/LinearAllocator.cpp
        Private/Memory/ListAllocator.cpp
        Private/Memory/TaggedHeap.cpp
        Private/Memory/Allocator.cpp
        Public/Memory/PoolAllocator.h
        Public/Memory/StackAllocator.h
        Public/Memory/LinearAllocator.h
        Public/Memory/ListAllocator.h
        Public/Memory/TaggedHeap.h
        Public/Memory/Allocator.h

        # Math submoduule's files

        Private/Math/MathUtility.cpp
        Private/Math/Radians.cpp
        Private/Math/Degrees.cpp
        Public/Math/MathUtility.h
        Public/Math/TQuat.h
        Public/Math/TVector2.h
        Public/Math/TVector3.h
        Public/Math/TVector4.h
        Public/Math/TMatrix2x2.h
        Public/Math/TMatrix3x3.h
        Public/Math/TMatrix4x4.h
        Public/Math/MathInclude.h
        Public/Math/Transform.h
        Public/Math/Rotation.h
        Public/Math/Sphere.h
        Public/Math/Plane.h
        Public/Math/AABB.h
        Public/Math/Radians.h
        Public/Math/Degrees.h
        Public/Math/Frustum.h

        # Strings submodule's files

        Private/Strings/StringPool.cpp
        Private/Strings/StringTable.cpp
        Public/Strings/StringStream.h
        Public/Strings/StringTable.h
        Public/Strings/DynamicString.h
        Public/Strings/StaticString.h
        Public/Strings/StringUtility.h
        Public/Strings/StringInclude.h
        Public/Strings/StringPool.h

        # XML submodule's files

        Private/XML/XMLUtility.cpp
        Public/XML/XMLNode.h
        Public/XML/XMLDocument.h
        Public/XML/XMLAttribute.h
        Public/XML/XMLUtility.h

        # Containers submodule's files

        Public/Containers/LinkedList.h
        Public/Containers/ArrayList.h
        Public/Containers/LinkedQueue.h
        Public/Containers/SharedList.h
        Public/Containers/HashMap.h

        # Threading submodule's files

        Private/Threading/ThreadPool.cpp
        Private/Threading/Thread.cpp
        Public/Threading/IRunnable.h
        Public/Threading/Thread.h
        Public/Threading/Future.h
        Public/Threading/ThreadPool.h
        Public/Threading/ConcurrentLinkedQueue.h

        Public/Time/Timer.h)

# Create static Core Subsystem library

add_library(BerserkCore STATIC ${SOURCE_LIB})