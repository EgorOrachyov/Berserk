/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ALLOCATOR_HPP
#define BERSERK_ALLOCATOR_HPP

#include <BerserkCore/Platform/Memory.hpp>

namespace Berserk {

    /**
     * Wraps default platform allocator
     */
    class Allocator {
    public:
        using Pointer = void *;
        using Size = size_t;

        Pointer Allocate(Size sizeInBytes);
        void Deallocate(Pointer memory);
    };

}

#endif //BERSERK_ALLOCATOR_HPP