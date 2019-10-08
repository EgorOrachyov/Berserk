cmake_minimum_required(VERSION 3.9)
project(BerserkCoreSystem)

# Core Subsystem files

set(
        SOURCE_LIB

        Public/HAL/Types.h
        Public/HAL/Platform.h
        Public/HAL/Memory.h

        Public/Platform/Mac/MacPlatform.h
        Public/Platform/Mac/MacPlatformMemory.h
        Public/Platform/Mac/MacPlatformTypes.h
        Private/Platform/Mac/MacPlatformMemory.cpp

        Public/Misc/Error.h
        Public/Misc/MathUtils.h

        Public/TComparator.h
        Public/TVariant.h
)

# Create static Core Subsystem library

add_library(BerserkCoreSystem STATIC ${SOURCE_LIB})