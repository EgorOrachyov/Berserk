/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/TPtrShared.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {

    namespace Details {

        void* PtrGenericAllocate(uint64 size) {
            Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
            return allocator? allocator->Allocate(size): nullptr;
        }

        void PtrGenericFree(void* memory) {
            Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
            if (allocator) allocator->Free(memory);
        }

        void* PtrMetaAllocate(uint64 size) {
            Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
            return allocator? allocator->Allocate(size): nullptr;
        }

        void PtrMetaFree(void* memory) {
            Allocator* allocator = Application::GetSingleton().GetGlobalAllocator();
            if (allocator) allocator->Free(memory);
        }

    }

}