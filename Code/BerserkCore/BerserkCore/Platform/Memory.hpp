/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_MEMORY_HPP
#define BERSERK_MEMORY_HPP

#include <BerserkCore/Typedefs.hpp>

namespace Berserk {
    namespace Platform {

        class Memory {
        public:
            static void Copy(void* destination, const void* source, size_t sizeInBytes);
            static void Set(void* destination, uint32 value, size_t sizeInBytes);
            static void* Allocate(size_t sizeInBytes);
            static void Deallocate(void* memory);
        };

    }
}

#endif //BERSERK_MEMORY_HPP