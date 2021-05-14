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
            auto& allocator = Application::Get().GetSystem().GetAllocator();
            return allocator.Allocate(size);
        }

        void PtrGenericFree(void* memory) {
            auto& allocator = Application::Get().GetSystem().GetAllocator();
            allocator.Free(memory);
        }

        void* PtrMetaAllocate(uint64 size) {
            auto& allocator = Application::Get().GetSystem().GetAllocator();
            return allocator.Allocate(size);
        }

        void PtrMetaFree(void* memory) {
            auto& allocator = Application::Get().GetSystem().GetAllocator();
            allocator.Free(memory);
        }

    }

}