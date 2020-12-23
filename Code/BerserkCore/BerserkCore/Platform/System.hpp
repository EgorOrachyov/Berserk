/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_HPP
#define BERSERK_SYSTEM_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Platform/Memory.hpp>
#include <BerserkCore/Misc/Singleton.hpp>
#include <BerserkCore/Misc/DateTime.hpp>

namespace Berserk {

    template <typename T>
    class PtrShared;
    class Application;

    namespace Platform {

        class System {
        public:
            /**
             * Query system date info
             * @param type Specifies whether must interpret as local or global time
             * @return Date info
             */
            static Date GetDate(TimeType type = TimeType::Local) {
                return Impl::Instance().GetDate(type);
            }

            /**
             * Query system time info
             * @param type Specifies whether must interpret as local or global time
             * @return Time info
             */
            static Time GetTime(TimeType type = TimeType::Local) {
                return Impl::Instance().GetTime(type);
            }

        protected:

            template<typename T>
            friend class ::Berserk::PtrShared;
            friend class ::Berserk::String;
            friend class ::Berserk::Application;
            friend class ::Berserk::Platform::Memory;

            class Impl: public Singleton<Impl> {
            public:
                virtual ~Impl() noexcept = default;

                /* General purpose memory management */
                virtual void* Allocate(size_t sizeInBytes) = 0;
                virtual void Deallocate(void* memory) = 0;

                virtual uint64 GetAllocateCallsCount() const = 0;
                virtual uint64 GetDeallocateCallsCount() const = 0;

                /* String buffers memory management */
                virtual void* AllocateStringBuffer(size_t sizeInBytes) = 0;
                virtual void DeallocateStringBuffer(void* buffer) = 0;

                /* Shared pointer meta info buffer memory management */
                virtual void* AllocatePtrMeta(size_t sizeInBytes) = 0;
                virtual void DeallocatePtrMeta(void* buffer) = 0;

                virtual Date GetDate(TimeType type) = 0;
                virtual Time GetTime(TimeType type) = 0;
            };

        };

    }
}

#endif //BERSERK_SYSTEM_HPP