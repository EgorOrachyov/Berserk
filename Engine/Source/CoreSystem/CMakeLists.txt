cmake_minimum_required(VERSION 3.9)
project(BerserkCoreSystem)

# Core Subsystem files

set(SOURCE_LIB

        # Misc submodule's files

        Private/Misc/FileUtility.cpp
        Private/Misc/Buffers.cpp
        Private/Misc/Crc32.cpp
        Public/Utility/FileUtility.h
        Public/Misc/Assert.h
        Public/Misc/AssertDev.h
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
        Public/Misc/Bits.h
        Public/Misc/Inline.h
        Public/Misc/Compilation.h

        # Logging submodule's files

        Public/Logging/ELogVerbosity.h
        Public/Logging/ILogManager.h
        Public/Logging/LogManager.h
        Public/Logging/Debug.h
        Public/Logging/DebugLogMacros.h

        # Memory submodule's files

        Private/Memory/PoolAllocator.cpp
        Private/Memory/StackAllocator.cpp
        Private/Memory/LinearAllocator.cpp
        Private/Memory/ListAllocator.cpp
        Private/Memory/TaggedHeap.cpp
        Private/Memory/Allocator.cpp
        Private/Memory/IAllocator.cpp
        Private/Memory/ProxyAllocator.cpp
        Public/Memory/PoolAllocator.h
        Public/Memory/StackAllocator.h
        Public/Memory/LinearAllocator.h
        Public/Memory/ListAllocator.h
        Public/Memory/TaggedHeap.h
        Public/Memory/Allocator.h
        Public/Memory/IAllocator.h
        Public/Memory/ProxyAllocator.h

        # Math submoduule's files

        Private/Math/Vec2f.cpp
        Private/Math/Vec3f.cpp
        Private/Math/Vec4f.cpp
        Private/Math/Quatf.cpp
        Private/Math/Mat2x2f.cpp
        Private/Math/Mat3x3f.cpp
        Private/Math/Mat4x4f.cpp
        Private/Math/AABB.cpp
        Private/Math/Sphere.cpp
        Private/Math/Plane.cpp
        Private/Math/Frustum.cpp
        Private/Math/Transform.cpp
        Private/Math/Rotation.cpp
        Private/Math/MathUtility.cpp
        Private/Math/Radians.cpp
        Private/Math/Degrees.cpp
        Public/Math/MathUtility.h
        Public/Math/Quatf.h
        Public/Math/Vec2f.h
        Public/Math/Vec3f.h
        Public/Math/Vec4f.h
        Public/Math/Mat2x2f.h
        Public/Math/Mat3x3f.h
        Public/Math/Mat4x4f.h
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
        Public/Strings/String.h

        # Containers submodule's files

        Public/Containers/LinkedList.h
        Public/Containers/ArrayList.h
        Public/Containers/LinkedQueue.h
        Public/Containers/SharedList.h
        Public/Containers/HashMap.h
        Public/Containers/TIterator.h
        Public/Containers/TList.h
        Public/Containers/TArray.h
        Public/Containers/TMap.h
        Public/Containers/TLinkedList.h

        # Threading submodule's files

        Private/Threading/ThreadPool.cpp
        Private/Threading/Thread.cpp
        Public/Threading/IRunnable.h
        Public/Threading/Thread.h
        Public/Threading/Future.h
        Public/Threading/ThreadPool.h
        Public/Threading/ConcurrentLinkedQueue.h
        Public/Threading/Mutex.h

        # Time submodule's files

        Private/Time/Timer.cpp
        Public/Time/Timer.h

        # Object submodule's files

        Public/Object/NewDelete.h
        Public/Object/Property.h
        Public/Object/StaticFunctions.h

        # Utility submodule's files

        Private/Utility/ProfilingUtility.cpp
        Public/Utility/ProfilingUtility.h
        Public/Utility/ProfilingMacro.h
        Public/Utility/Printer.h

        # Resource submodule's files

        Public/Resource/IResource.h
        Public/Resource/IPointer.h
        Public/Resource/UniquePointer.h

        # Info submodule's files

        Public/Info/AudioDriver.h
        Public/Info/VideoDriver.h
        Public/Info/ImageImporter.h

        # Serialization submodule's files

        Private/Serialization/ArchiveWriter.cpp
        Public/Serialization/Archive.h
        Public/Serialization/ArchiveWriter.h

        # IO submodule's files

        Private/IO/CachedFileWriter.cpp
        Private/IO/LoggedFile.cpp
        Private/IO/PlatformFile.cpp
        Private/IO/PlatformFileDev.cpp
        Public/IO/CachedFileWriter.h
        Public/IO/LoggedFile.h
        Public/IO/IFile.h
        Public/IO/PlatformFile.h
        Public/IO/PlatformFileDev.h

        # Exception submodule's files

        Public/Exception/Exception.h
        Public/Exception/CoreException.h

        )

# Create static Core Subsystem library

add_library(BerserkCoreSystem STATIC ${SOURCE_LIB})