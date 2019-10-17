#--------------------------------------------------------------------------------#
# This file is part of:                                                          #
# Berserk Engine project                                                         #
# https://github.com/EgorOrachyov/Berserk                                        #
#--------------------------------------------------------------------------------#
# MIT License                                                                    #
#                                                                                #
# Copyright (c) 2018-2019 Egor Orachyov                                          #
#                                                                                #
# Permission is hereby granted, free of charge, to any person obtaining a copy   #
# of this software and associated documentation files (the "Software"), to deal  #
# in the Software without restriction, including without limitation the rights   #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      #
# copies of the Software, and to permit persons to whom the Software is          #
# furnished to do so, subject to the following conditions:                       #
#                                                                                #
# The above copyright notice and this permission notice shall be included in all #
# copies or substantial portions of the Software.                                #
#                                                                                #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  #
# SOFTWARE.                                                                      #
#--------------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.1)
project(BerserkCore)

set(
        SOURCE_LIB

        Public/HAL/Platform.h
        Public/HAL/Memory.h
        Public/HAL/Lock.h
        Public/HAL/TypesCheck.h

        Public/Platform/Mac/MacPlatform.h
        Private/Platform/Mac/MacPlatformMemory.cpp
        Public/Platform/Mac/MacPlatformMemory.h
        Public/Platform/Mac/MacPlatformTypes.h
        Public/Platform/Mac/MacPlatformLock.h

        Public/Platform/Linux/LinuxPlatform.h
        Public/Platform/Linux/LinuxPlatformLock.h
        Private/Platform/Linux/LinuxPlatformMemory.cpp
        Public/Platform/Linux/LinuxPlatformMemory.h
        Public/Platform/Linux/LinuxPlatformTypes.h

        Private/Misc/Crc32.cpp
        Public/Misc/Crc32.h
        Public/Misc/Error.h
        Private/Misc/MathUtils.cpp
        Public/Misc/MathUtils.h
        Public/Misc/PrinterUtils.h
        Public/Misc/StringUtils.h
        Public/Misc/Assertion.h

        Public/Memory/IAllocator.h
        Private/Memory/Allocator.cpp
        Public/Memory/Allocator.h
        Public/Memory/StaticAllocator.h

        Public/Containers/TList.h
        Public/Containers/TStaticArray.h
        Public/Containers/TArray.h
        Public/Containers/TLinkedList.h
        Public/Containers/TListMap.h

        Public/TComparator.h
        Public/TVariant.h
        Public/TPredicate.h
        Public/TIterable.h
)

add_library(BerserkCore STATIC ${SOURCE_LIB})