/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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