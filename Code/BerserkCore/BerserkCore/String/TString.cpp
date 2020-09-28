/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/String/TString.hpp>
#include <BerserkCore/Application.hpp>

namespace Berserk {
    namespace Details {

        void* AllocateString(uint32 size) {
            return Application::GetSingleton().GetGlobalAllocator().Allocate(size);
        }

        void FreeString(void* block) {
            Application::GetSingleton().GetGlobalAllocator().Free(block);
        }

        void AlignString(uint32 &capacity, uint32 base) {
            // Can assign what ever we want
            // It would be better to set as min the cache line size
            uint32 bytesSize = 64;
            uint32 factor = 2;

            while (bytesSize < capacity * base) {
                bytesSize *= factor;
            }

            capacity = bytesSize / base;
        }

    }
}