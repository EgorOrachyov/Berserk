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
#include <BerserkUnix/UnixDialogs.hpp>
#include <BerserkUnix/UnixConsole.hpp>
#include <BerserkUnix/UnixFileSystem.hpp>
#include <BerserkUnix/UnixStringTable.hpp>
#include <BerserkUnix/UnixThreadManager.hpp>

#ifdef BERSERK_WITH_OPENGL
#include <BerserkOpenGL/GLDriver.hpp>
#endif

namespace Berserk {

    class UnixSystem : public System {
    public:

        class UnixImpl: public System::Impl {
        public:
            static constexpr const char* DEFAULT_LOCALE = "en_US.UTF-8";

            UnixImpl();
            ~UnixImpl() noexcept override;

            void InitializeRHI();

            void *Allocate(size_t sizeInBytes) override;
            void *Reallocate(void *memory, size_t sizeInBytes) override;
            void Deallocate(void *memory) override;

            void *AllocateStringBuffer(size_t sizeInBytes) override;
            void DeallocateStringBuffer(void *buffer, size_t sizeInBytes) override;

            void *AllocatePtrMeta(size_t sizeInBytes) override;
            void DeallocatePtrMeta(void *buffer) override;

            Date GetDate(TimeType type) override;
            Time GetTime(TimeType type) override;
            TimeStamp GetTimeStamp() override;
            void GetDateTime(TimeStamp timeStamp, Date &date, Time &time, TimeType timeType) override;

            TextWriter &GetOutStream() override;
            TextWriter &GetErrorStream() override;
            Log &GetLogger() override;

            void Abort() override;
            void FixedUpdate() override;

        private:
            PoolsAllocator* mStringsPool = nullptr;
            UnixStringTable::UnixImpl* mStringTable = nullptr;
            UnixFileSystem::UnixImpl* mFileSystem = nullptr;
            UnixThreadManager::UnixImpl* mThreadManager = nullptr;
            UnixDialogs::UnixImpl* mDialogs = nullptr;
            GlfwContext* mGlfwContext = nullptr;

#ifdef BERSERK_WITH_OPENGL
            RHI::GLDriver::GLImpl* mRHIImpl = nullptr;
#endif //BERSERK_WITH_OPENGL

            TextWriter* mConsoleOut = nullptr;
            TextWriter* mConsoleError = nullptr;
            Log* mLogger = nullptr;

            volatile uint32 mExitCode = 0;
        };
    };
}

#endif //BERSERK_UNIXSYSTEM_HPP