/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_UNIXSYSTEM_HPP
#define BERSERK_UNIXSYSTEM_HPP

#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Memory/PoolsAllocator.hpp>
#include <BerserkGlfw/GlfwContext.hpp>
#include <BerserkUnix/UnixConsole.hpp>
#include <BerserkUnix/UnixFileSystem.hpp>
#include <BerserkUnix/UnixStringTable.hpp>
#include <BerserkUnix/UnixThreadManager.hpp>

namespace Berserk {
    namespace Platform {

        class UnixSystem : public System {
        public:

            class UnixImpl: public System::Impl {
            public:
                static constexpr const char* DEFAULT_LOCALE = "en_US.UTF-8";

                UnixImpl();
                ~UnixImpl() noexcept override;

                void *Allocate(size_t sizeInBytes) override;
                void Deallocate(void *memory) override;

                uint64 GetAllocateCallsCount() const override;
                uint64 GetDeallocateCallsCount() const override;

                void *AllocateStringBuffer(size_t sizeInBytes) override;
                void DeallocateStringBuffer(void *buffer, size_t sizeInBytes) override;

                void *AllocatePtrMeta(size_t sizeInBytes) override;
                void DeallocatePtrMeta(void *buffer) override;

                Date GetDate(TimeType type) override;
                Time GetTime(TimeType type) override;
                TimeStamp GetTimeStamp() override;
                void GetDateTime(TimeStamp timeStamp, Date &date, Time &time, TimeType timeType) override;

                const Array<String> &GetCmdArgs() const override;
                const String &GetLocale() const override;

                bool IsOutputPresented() const override;
                TextWriter &GetOutStream() override;
                TextWriter &GetErrorStream() override;
                Log &GetLogger() override;

                void Abort() override;
                bool HasNativeGui() const override;

                void FixedUpdate() override;

                template<typename T, typename ... TArgs>
                T* Create(TArgs&& ... args) {
                    auto mem = Allocate(sizeof(T));
                    return new (mem) T(std::forward<TArgs>(args)...);
                }

                template<typename T>
                void Release(T* system) {
                    if (system != nullptr) {
                        system->~T();
                        Deallocate(system);
                    }
                }

            private:
                AtomicUint64 mAllocCalls;
                AtomicUint64 mDeallocCalls;

                PoolsAllocator* mStringsPool = nullptr;
                UnixStringTable::UnixImpl* mStringTable = nullptr;
                UnixFileSystem::UnixImpl* mFileSystem = nullptr;
                UnixThreadManager::UnixImpl* mThreadManager = nullptr;
                GlfwContext* mGlfwContext = nullptr;

                Array<String> mCmdArgs;
                String mLocale;

                TextWriter* mConsoleOut = nullptr;
                TextWriter* mConsoleError = nullptr;
                Log* mLogger = nullptr;
                bool mIsOutputPresented = true;
                bool mIsGuiElementsProvided = true;

                volatile uint32 mExitCode = 0;
            };
        };

    }
}

#endif //BERSERK_UNIXSYSTEM_HPP