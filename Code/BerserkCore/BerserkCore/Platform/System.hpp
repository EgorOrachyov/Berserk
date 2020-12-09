/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/String/String.hpp>
#include <BerserkCore/Typedefs.hpp>

namespace Berserk {
    namespace Platform {

        class System: public Misc::Singleton<System> {
        public:
            /**
             * Allocates memory within system memory allocator
             * @param sizeInBytes Size of the memory buffer to allocate
             * @return Buffer pointer
             */
            virtual void* Allocate(size_t sizeInBytes) = 0;

            /**
             * Deallocate system memory buffer
             * @param memory Buffer to deallocate
             */
            virtual void Deallocate(void* memory) = 0;

        protected:
            /**
             * For string class only
             */
            friend class String::String;

            /** Allocates string buffer */
            virtual void* AllocateStringBuffer(size_t sizeInBytes) = 0;

            /** Deallocates string buffer */
            virtual void DeallocateStringBuffer(void* buffer) = 0;

        };

    }
}

#endif //BERSERK_SYSTEM_HPP