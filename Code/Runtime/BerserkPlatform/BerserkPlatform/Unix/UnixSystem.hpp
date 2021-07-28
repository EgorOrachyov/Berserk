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
#include <BerserkPlatform/PlatformDialogs.hpp>
#include <BerserkPlatform/PlatformConsole.hpp>
#include <BerserkPlatform/PlatformStringTable.hpp>
#include <BerserkPlatform/PlatformThreadManager.hpp>
#include <BerserkPlatform/PlatformTaskManager.hpp>
#include <BerserkPlatform/Glfw/GlfwContext.hpp>
#include <BerserkPlatform/Unix/UnixFileSystem.hpp>

namespace Berserk {

    class UnixSystem : public System {
    public:

        class UnixImpl final: public System::Impl {
        public:
            const uint32 TASK_MAN_WORKERS_COUNT = 2;
            const char* DEFAULT_LOCALE = "en_US.UTF-8";

            UnixImpl();
            ~UnixImpl() noexcept override;

            void InitializeRHI();

            void *Allocate(uint64 sizeInBytes) override;
            void *Reallocate(void *memory, uint64 sizeInBytes) override;
            void Deallocate(void *memory) override;

            void *AllocateStringBuffer(uint64 sizeInBytes) override;
            void DeallocateStringBuffer(void *buffer, uint64 sizeInBytes) override;

            void *AllocatePtrMeta(uint64 sizeInBytes) override;
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
            PlatformStringTable::PImpl* mStringTable = nullptr;
            UnixFileSystem::UnixImpl* mFileSystem = nullptr;
            PlatformThreadManager::PImpl* mThreadManager = nullptr;
            PlatformTaskManager::PImpl* mTaskManager = nullptr;
            PlatformDialogs::PImpl* mDialogs = nullptr;
            GlfwContext* mGlfwContext = nullptr;
            TextWriter* mConsoleOut = nullptr;
            TextWriter* mConsoleError = nullptr;
            Log* mLogger = nullptr;

            bool mGlfwNoClientApi = false;

#ifdef BERSERK_WITH_OPENGL
            RHI::GLDriver::GLImpl* mGlRHIImpl = nullptr;
#endif

#ifdef BERSERK_WITH_VULKAN
            RHI::VulkanDriver::VkImpl* mVkRHIImpl = nullptr;
#endif

            volatile uint32 mExitCode = 0;
        };
    };
}

#endif //BERSERK_UNIXSYSTEM_HPP