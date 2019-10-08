cmake_minimum_required(VERSION 3.9)
project(BerserkCoreSystem)

# Core Subsystem files

set(
        SOURCE_LIB

        Public/HAL/Platform.h
        Public/HAL/Memory.h

        Public/Platform/Mac/MacPlatform.h
        Public/Platform/Mac/MacPlatformMemory.h
        Private/Platform/Mac/MacPlatformMemory.cpp
        Public/Platform/Mac/MacPlatformTypes.h

        Public/Misc/Crc32.h
        Private/Misc/Crc32.cpp
        Public/Misc/Error.h
        Public/Misc/MathUtils.h
        Public/Misc/PrinterUtils.h
        Public/Misc/StringUtils.h
        Public/Misc/Assertion.h

        Public/Containers/TList.h
        Public/Containers/TStaticArray.h
        # Public/Containers/TArray.h

        Public/TComparator.h
        Public/TVariant.h


        Public/TPredicate.h)

# Create static Core Subsystem library

add_library(BerserkCoreSystem STATIC ${SOURCE_LIB})