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
        Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
        return allocator? allocator->Allocate(size): nullptr;
    }

    void GlobalAllocator::Free(void *memory) {
        Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
        if (allocator) allocator->Free(memory);
    }

}