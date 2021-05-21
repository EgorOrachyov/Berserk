/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_UNIXSYSTEM_HPP
#define BERSERK_UNIXSYSTEM_HPP

#ifdef BERSERK_WITH_OPENGL
#include <BerserkOpenGL/GLDriver.hpp>
#endif

#ifdef BERSERK_WITH_VULKAN
#include <BerserkVulkan/VulkanDriver.hpp>
#endif

#include <BerserkCore/Platform/System.hpp>
#include <BerserkCore/Platform/Atomic.hpp>
#include <BerserkCore/Memory/PoolsAllocator.hpp>
#include <BerserkGlfw/GlfwContext.hpp>
#include <BerserkUnix/UnixDialogs.hpp>
#include <BerserkUnix/UnixConsole.hpp>
#include <BerserkUnix/UnixFileSystem.hpp>
#include <BerserkUnix/UnixStringTable.hpp>
#include <BerserkUnix/UnixThreadManager.hpp>

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
            RHI::GLDriver::GLImpl* mGlRHIImpl = nullptr;
#endif

#ifdef BERSERK_WITH_VULKAN
            RHI::VulkanDriver::VkImpl* mVkRHIImpl = nullptr;
#endif

            TextWriter* mConsoleOut = nullptr;
            TextWriter* mConsoleError = nullptr;
            Log* mLogger = nullptr;

            volatile uint32 mExitCode = 0;
        };
    };
}

#endif //BERSERK_UNIXSYSTEM_HPP