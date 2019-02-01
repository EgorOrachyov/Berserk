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
        Public/Misc/Cast.h
        Public/Misc/Crc32.h
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
        Public/Math/MathUtility.h

        # Strings submodule's files

        Private/Strings/StringTable.cpp
        Private/Strings/DynamicString.cpp
        Public/Strings/StringStream.h
        Public/Strings/StringTable.h
        Public/Strings/DynamicString.h
        Public/Strings/StringUtility.h

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


        Public/Math/Vector2.h Public/Math/Vector3.h Public/Math/Vector4.h Public/Math/Matrix2x2.h Public/Math/Matrix3x3.h Public/Math/Matrix4x4.h Public/Math/MathInclude.h Private/Math/AABB.cpp Public/Math/AABB.h Private/Math/Rotator.cpp Public/Math/Rotator.h Private/Math/Transform.cpp Public/Math/Transform.h Private/Math/Sphere.cpp Public/Math/Sphere.h Private/Math/Plane.cpp Public/Math/Plane.h)

# Create static Core Subsystem library

add_library(BerserkCore STATIC ${SOURCE_LIB})