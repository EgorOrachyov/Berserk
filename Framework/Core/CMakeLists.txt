cmake_minimum_required(VERSION 3.9)
project(BerserkRCore)

# Core Subsystem files

set(SOURCE_LIB

        # Misc submodule's files

        Private/Misc/Buffers.cpp
        Public/Misc/Assert.h
        Public/Misc/Buffers.h
        Public/Misc/Types.h
        Public/Misc/Platform.h
        Public/Misc/Alignment.h
        Public/Misc/Include.h
        Public/Misc/Delete.h
        Public/Misc/UsageDescriptors.h
        Public/Misc/Cast.h
        Public/Misc/Compilation.h
        Public/Test.h

        # Logging submodule's files

        Private/Logging/LogManager.cpp
        Public/Logging/LogVerbosity.h
        Public/Logging/LogMacros.h
        Public/Logging/LogManager.h

        # Memory submodule's files

        Private/Memory/PoolAllocator.cpp Public/Memory/PoolAllocator.h Private/Memory/StackAllocator.cpp Public/Memory/StackAllocator.h Private/Memory/LinearAllocator.cpp Public/Memory/LinearAllocator.h Private/Memory/ListAllocator.cpp Public/Memory/ListAllocator.h Private/Memory/TaggedHeap.cpp Public/Memory/TaggedHeap.h Private/Memory/Allocator.cpp Public/Memory/Allocator.h)

# Create static Core Subsystem library

add_library(BerserkRCore STATIC ${SOURCE_LIB})