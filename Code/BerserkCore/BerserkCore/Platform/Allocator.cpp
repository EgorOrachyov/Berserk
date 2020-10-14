/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Platform/Allocator.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {

    void *GlobalAllocator::Allocate(uint64 size) {
        auto& allocator = Application::Get().GetSystem().GetAllocator();
        return allocator.Allocate(size);
    }

    void GlobalAllocator::Free(void *memory) {
        auto& allocator = Application::Get().GetSystem().GetAllocator();
        allocator.Free(memory);
    }

}