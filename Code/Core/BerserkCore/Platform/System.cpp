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

#include <BerserkCore/Platform/System.hpp>

namespace Berserk {

    uint64 System::Impl::GetAllocateCallsCount() const {
        return mAllocCalls.load();
    }

    uint64 System::Impl::GetDeallocateCallsCount() const {
        return mDeallocCalls.load();
    }

    const Array<String> &System::Impl::GetCmdArgs() const {
        return mCmdLine;
    };

    const String &System::Impl::GetLocale() const {
        return mLocale;
    };

    bool System::Impl::IsStdoutEnabled() const {
        return mStdoutEnabled;
    };

    bool System::Impl::IsStderrEnabled() const {
        return mStderrEnabled;
    };

    bool System::Impl::IsWindowsEnabled() const {
        return mWindowsEnabled;
    };

    bool System::Impl::IsHdpiEnabled() const {
        return mHdpiEnabled;
    };

    bool System::Impl::UseVsync() const {
        return mUseVsync;
    };
    
}