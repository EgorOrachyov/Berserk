/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ALLOCATOR_HPP
#define BERSERK_ALLOCATOR_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    class Allocator {
    public:
        virtual ~Allocator() = default;
        virtual void* allocate(uint64 size) = 0;
        virtual void free(void* memory) = 0;
    };

}

#endif //BERSERK_ALLOCATOR_HPP