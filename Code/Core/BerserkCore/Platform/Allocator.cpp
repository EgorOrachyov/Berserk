/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Platform/Memory.hpp>

namespace Berserk {

    Allocator::Pointer Allocator::Allocate(Size sizeInBytes) {
        return Memory::Allocate(sizeInBytes);
    }

    void Allocator::Deallocate(Pointer memory) {
        Memory::Deallocate(memory);
    }

}