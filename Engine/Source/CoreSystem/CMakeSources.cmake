set(
        SOURCE_LIB


        Private/Misc/Crc32.cpp
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
        Public/Misc/Exception.h
        Public/Misc/NewDelete.h

        Private/Logging/LogManager.cpp
        Public/Logging/ELogVerbosity.h
        Public/Logging/ILogManager.h
        Public/Logging/LogManager.h
        Public/Logging/Debug.h
        Public/Logging/DebugLogMacros.h
        Public/Logging/LogMacros.h

        Private/Memory/Allocator.cpp
        Private/Memory/PoolAllocator.cpp
        Private/Memory/ListAllocator.cpp
        Private/Memory/LinearAllocator.cpp
        Public/Memory/Allocator.h
        Public/Memory/IAllocator.h
        Public/Memory/PoolAllocator.h
        Public/Memory/ListAllocator.h
        Public/Memory/LinearAllocator.h

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
        Private/Math/Angles.cpp
        Private/Math/MathUtility.cpp
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
        Public/Math/Frustum.h
        Public/Math/Rect.h
        Public/Math/Size.h
        Public/Math/Angles.h

        Public/Containers/TIterator.h
        Public/Containers/TList.h
        Public/Containers/TArray.h
        Public/Containers/TMap.h
        Public/Containers/TLinkedList.h
        Public/Containers/TSet.h
        Public/Containers/THashMap.h
        Public/Containers/THashSet.h

        Public/Threading/Mutex.h
        Public/Threading/Thread.h
        Public/Threading/IRunnable.h
        Public/Threading/IThreadManager.h
        Private/Threading/ThreadManager.cpp
        Public/Threading/ThreadManager.h
        Public/Threading/AtomicTypes.h
        Public/Containers/TRemoveIterator.h

        Private/Resource/PtrManager.cpp
        Public/Resource/TUniquePtr.h
        Public/Resource/TSharedPtr.h
        Public/Resource/PtrManager.h
        Public/Resource/TWeakPtr.h

        Public/CMakeOptions/CMakeOptionsList.h

        Public/IO/Printer.h
        Private/IO/CachedFileWriter.cpp
        Private/IO/LoggedFile.cpp
        Private/IO/PlatformFile.cpp
        Private/IO/PlatformFileDev.cpp
        Public/IO/CachedFileWriter.h
        Public/IO/LoggedFile.h
        Public/IO/IFile.h
        Public/IO/PlatformFile.h
        Public/IO/PlatformFileDev.h
        Public/IO/IOutputDevice.h
        Public/IO/OutputDevice.h

        Private/Strings/StringManager.cpp
        Private/Strings/StringDynamic.cpp
        Private/Strings/String.cpp
        Public/Strings/StringUtility.h
        Public/Strings/StringManager.h
        Public/Strings/StringStatic.h
        Public/Strings/String.h
        Public/Strings/StringDynamic.h

        Public/Time/Clock.h
        Public/Time/Timer.h

        Private/Serialization/ArchiveFileWriter.cpp
        Private/Serialization/ArchiveFileReader.cpp
        Public/Serialization/ArchiveWriter.h
        Public/Serialization/ArchiveReader.h
        Public/Serialization/ArchiveFileWriter.h
        Public/Serialization/ArchiveFileReader.h

        Private/Console/ConsoleManager.cpp
        Public/Console/IConsoleManager.h
        Public/Console/IConsoleVariable.h
        Public/Console/IConsoleCommand.h
        Public/Console/IConsoleObject.h
        Public/Console/ConsoleManager.h
        Private/Console/ConsoleVariable.h
        Private/Console/ConsoleCommand.h

)