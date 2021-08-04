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

#include <BerserkCore/Platform/EntryPoint.hpp>

#if defined(BERSERK_TARGET_MACOS) || defined(BERSERK_TARGET_LINUX)
#include <BerserkPlatform/Unix/UnixSystem.hpp>
#endif

#if defined(BERSERK_TARGET_WINDOWS)
#include <BerserkPlatform/Windows/WindowsSystem.hpp>
#endif

namespace Berserk {

#if defined(BERSERK_TARGET_WINDOWS)
    using PlatfromType = WindowsSystem::WinImpl;
#elif defined(BERSERK_TARGET_MACOS) || defined(BERSERK_TARGET_LINUX)
    using PlatfromType = UnixSystem::UnixImpl;
#endif

    static uint8 MemoryBuffer[sizeof(PlatfromType)];
    static PlatfromType* gPlatform = nullptr;

    void EntryPoint::PlatformInitialize() {
        gPlatform = new ((void *) MemoryBuffer) PlatfromType();
    }

    void EntryPoint::PlatformInitializeRHI() {
        gPlatform->InitializeRHI();
    }

    void EntryPoint::FixedUpdate() {
        gPlatform->FixedUpdate();
    }

    void EntryPoint::PlatformFinalize() {
        gPlatform->~PlatfromType();
        gPlatform = nullptr;
    }

}