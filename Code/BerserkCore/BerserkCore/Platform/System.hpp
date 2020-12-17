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
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Typedefs.hpp>

namespace Berserk {

    template <typename T>
    class PtrShared;
    class Application;

    namespace Platform {

        class Memory;

        class System: public Singleton<System> {
        public:
            virtual ~System() = default;

        protected:

            template<typename T>
            friend class ::Berserk::PtrShared;
            friend class ::Berserk::String;
            friend class ::Berserk::Application;
            friend class ::Berserk::Platform::Memory;


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

            /**
             * @return Calls count to allocate function
             */
            virtual uint64 GetAllocateCallsCount() const = 0;

            /**
             * @return Calls count to deallocate function
             */
            virtual uint64 GetDeallocateCallsCount() const = 0;

            /** Allocates string buffer */
            virtual void* AllocateStringBuffer(size_t sizeInBytes) = 0;

            /** Deallocates string buffer */
            virtual void DeallocateStringBuffer(void* buffer) = 0;

            /** Allocates shared pointer meta info buffer */
            virtual void* AllocatePtrMeta(size_t sizeInBytes) = 0;

            /** Deallocates shared pointer meta info buffer */
            virtual void DeallocatePtrMeta(void* buffer) = 0;

        };

    }
}

#endif //BERSERK_SYSTEM_HPP