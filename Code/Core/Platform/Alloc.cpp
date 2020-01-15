/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Platform/Alloc.h>
#include <Platform/Memory.h>

namespace Berserk {

    struct AllocDefault : public Alloc {
        ~AllocDefault() override = default;

        void *allocate(uint64 size) override {
            return Memory::allocate(size);
        }

        void free(void *memory) override {
            Memory::free(memory);
        }
    };

    static AllocDefault sAllocDefault;

    Alloc& Alloc::getSingleton() {
        return sAllocDefault;
    }

}