cmake_minimum_required(VERSION 3.9)
project(BerserkCoreSystem)

# Core Subsystem files

set(SOURCE_LIB

        # Misc submodule's files

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

        # Logging submodule's files

        Private/Logging/LogManager.cpp
        Public/Logging/ELogVerbosity.h
        Public/Logging/ILogManager.h
        Public/Logging/LogManager.h
        Public/Logging/Debug.h
        Public/Logging/DebugLogMacros.h

        # Memory submodule's files

        Private/Memory/Allocator.cpp
        Private/Memory/PoolAllocator.cpp
        Private/Memory/ListAllocator.cpp
        Public/Memory/Allocator.h
        Public/Memory/IAllocator.h
        Public/Memory/PoolAllocator.h
        Public/Memory/ListAllocator.h

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

        # Containers submodule's files

        Public/Containers/TIterator.h
        Public/Containers/TList.h
        Public/Containers/TArray.h
        Public/Containers/TMap.h
        Public/Containers/TLinkedList.h
        Public/Containers/TSet.h
        Public/Containers/THashMap.h
        Public/Containers/THashSet.h

        # Threading submodule's files

        Public/Threading/Mutex.h
        Public/Threading/Thread.h

        # Utility submodule's files

        Public/IO/Printer.h

        # Resource submodule's files

        Private/Resource/SharedPtrManager.cpp
        Public/Resource/IResource.h
        Public/Resource/TUniquePtr.h
        Public/Resource/TSharedPtr.h
        Public/Resource/SharedPtrManager.h

        # Info submodule's files

        Public/Info/AudioDriver.h
        Public/Info/VideoDriver.h
        Public/Info/ImageImporter.h

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
        Public/IO/IOutputDevice.h
        Public/IO/OutputDevice.h

        # Exception submodule's files

        # Strings submodule's files

        Private/Strings/StringManager.cpp
        Private/Strings/StringDynamic.cpp
        Public/Strings/StringUtility.h
        Public/Strings/StringManager.h
        Public/Strings/StringStatic.h
        Public/Strings/String.h
        Public/Strings/StringDynamic.h

        # Time submodule's files

        Public/Time/Clock.h
        Public/Time/Timer.h

        # Serialization submodule's files

        Private/Serialization/ArchiveFileWriter.cpp
        Private/Serialization/ArchiveFileReader.cpp
        Public/Serialization/ArchiveWriter.h
        Public/Serialization/ArchiveReader.h
        Public/Serialization/ArchiveFileWriter.h
        Public/Serialization/ArchiveFileReader.h

        # Console submodule's files

        Private/Console/ConsoleManager.cpp
        Public/Console/IConsoleManager.h
        Public/Console/IConsoleVariable.h
        Public/Console/IConsoleCommand.h
        Public/Console/ConsoleManager.h


        Public/Console/IConsoleObject.h)

# Create static Core Subsystem library

add_library(BerserkCoreSystem STATIC ${SOURCE_LIB})