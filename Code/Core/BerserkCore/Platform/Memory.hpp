/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_HPP
#define BERSERK_MEMORY_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    /**
     * @brief System Memory
     *
     * Wrapper for system memory allocation and manipulation.
     */
    class Memory {
    public:
        static const size_t ALIGNMENT = 16;
        static const size_t KiB = 1024;
        static const size_t MiB = 1024 * KiB;

        /**
         * Copy source to the destination region.
         * @note If sizeInBytes == 0 function does nothing.
         *
         * @param destination Where to copy data
         * @param source Data to copy
         * @param sizeInBytes Size of the source region to copy (in bytes)
         */
        static void Copy(void* destination, const void* source, size_t sizeInBytes);

        /**
         * Set destination bytes to value.
         * @note If sizeInBytes == 0 function does nothing.
         *
         * @param destination Where to set data
         * @param value Byte to set
         * @param sizeInBytes Size of the destination region to set (in bytes)
         */
        static void Set(void* destination, uint32 value, size_t sizeInBytes);

        /**
         * Dynamically allocates data by default system allocator.
         * @param sizeInBytes Non-zero size in byte to allocate
         *
         * @return Pointer to allocated memory
         */
        static void* Allocate(size_t sizeInBytes);

        /**
         * Reallocates previously allocated memory with Allocate() or Reallocate() functions.
         * @param memory Pointer to memory to reallocate
         * @param sizeInBytes Non-zero size in byte to reallocate
         *
         * @return
         */
        static void* Reallocate(void* memory, size_t sizeInBytes);

        /**
         * Deallocates previously allocated memory with Allocate() or Reallocate() functions.
         * @param memory Pointer to free
         */
        static void Deallocate(void* memory);

        /** @return Total number of allocate calls */
        static uint64 GetAllocateCalls();

        /** @return Total number of deallocate calls */
        static uint64 GetDeallocateCalls();

        /**
         * Align size to specified alignment.
         *
         * @param size Size in bytes to align
         * @param alignment Alignment in bytes (must be power of 2)
         *
         * @return Aligned size
         */
        static size_t AlignSize(size_t size, size_t alignment) {
            return size + (size % alignment == 0? 0: alignment - (size % alignment));
        }

        /**
         * Create object from args in the system allocated memory.
         * @note It is engine replacement for new/delete logic
         *
         * @tparam T Type of the object to create
         * @tparam TArgs Types of args
         * @param args Args to create object
         *
         * @return Pointer to allocated and constructed object
         */
        template<typename T, typename ... TArgs>
        static T* Make(TArgs&& ... args) {
            auto mem = Allocate(sizeof(T));
            T* result;

            try {
                result = new (mem) T(std::forward<TArgs>(args)...);
            }
            catch (const std::exception& e) {
                Deallocate(mem);
                throw;
            }

            return result;
        }

        /**
         * Destroy object created with Make() call.
         * @note It is engine replacement for new/delete logic
         * @note If object has no virtual destructor, then T must match T in Make() call

         * @tparam T  Type of the object to release
         * @param t  Object to release
         */
        template<typename T>
        static void Release(T* t) {
            try {
                t->~T();
            }
            catch (const std::exception& e) {
                Deallocate((void*) t);
                throw;
            }

            Deallocate((void*) t);
        }
    };
}

#endif //BERSERK_MEMORY_HPP