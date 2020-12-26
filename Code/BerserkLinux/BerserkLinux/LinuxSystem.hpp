/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_LINUXSYSTEM_HPP
#define BERSERK_LINUXSYSTEM_HPP

#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkLinux/LinuxConsole.hpp>
#include <BerserkLinux/LinuxFileSystem.hpp>

namespace Berserk {
    namespace Platform {

        class LinuxSystem : public System {
        public:

            class LinuxImpl: public System::Impl {
            public:
                LinuxImpl();
                ~LinuxImpl() noexcept override;

                void *Allocate(size_t sizeInBytes) override;
                void Deallocate(void *memory) override;

                uint64 GetAllocateCallsCount() const override;
                uint64 GetDeallocateCallsCount() const override;

                void *AllocateStringBuffer(size_t sizeInBytes) override;
                void DeallocateStringBuffer(void *buffer) override;

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

                template<typename T, typename ... TArgs>
                T* Create(TArgs&& ... args) {
                    auto mem = Allocate(sizeof(T));
                    return new (mem) T(std::forward<TArgs>(args)...);
                }

                template<typename T>
                void Release(T* system) {
                    system->~T();
                    Deallocate(system);
                }

            private:
                AtomicUint64 mAllocCalls;
                AtomicUint64 mDeallocCalls;

                LinuxFileSystem::LinuxImpl* mFileSystem;

                Array<String> mCmdArgs;
                String mLocale = "en_US.UTF-8";

                TextWriter* mConsoleOut;
                TextWriter* mConsoleError;
                Log* mLogger;
                bool mIsOutputPresented = true;

                volatile uint32 mExitCode = 0;
            };
        };

    }
}

#endif //BERSERK_LINUXSYSTEM_HPP