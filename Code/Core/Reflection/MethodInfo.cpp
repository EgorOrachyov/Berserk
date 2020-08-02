/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/MethodInfo.h>

namespace Berserk {

    MethodInfo::MethodInfo(CString name)
            :   mName(std::move(name)),
                mHasRetValue(false) {

    }

    MethodInfo::MethodInfo(CString name, Property arg1)
            :   mName(std::move(name)),
                mHasRetValue(false) {
        mArgs.emplace(std::move(arg1));
    }

    MethodInfo::MethodInfo(CString name, Property arg1, Property arg2)
            :   mName(std::move(name)),
                mHasRetValue(false) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
    }

    MethodInfo::MethodInfo(CString name, Property arg1, Property arg2, Property arg3)
            :   mName(std::move(name)),
                mHasRetValue(false) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
        mArgs.emplace(std::move(arg3));
    }

    MethodInfo::MethodInfo(CString name, Property arg1, Property arg2, Property arg3, Property arg4)
            :   mName(std::move(name)),
                mHasRetValue(false) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
        mArgs.emplace(std::move(arg3));
        mArgs.emplace(std::move(arg4));
    }

    MethodInfo::MethodInfo(Property ret, CString name)
            :   mName(std::move(name)),
                mHasRetValue(true),
                mRetVal(std::move(ret)) {

    }

    MethodInfo::MethodInfo(Property ret, CString name, Property arg1)
            :   mName(std::move(name)),
                mHasRetValue(true),
                mRetVal(std::move(ret)) {
        mArgs.emplace(std::move(arg1));
    }

    MethodInfo::MethodInfo(Property ret, CString name, Property arg1, Property arg2)
            :   mName(std::move(name)),
                mHasRetValue(true),
                mRetVal(std::move(ret)) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
    }

    MethodInfo::MethodInfo(Property ret, CString name, Property arg1, Property arg2, Property arg3)
            :   mName(std::move(name)),
                mHasRetValue(true),
                mRetVal(std::move(ret)) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
        mArgs.emplace(std::move(arg3));
    }

    MethodInfo::MethodInfo(Property ret, CString name, Property arg1, Property arg2, Property arg3, Property arg4)
            :   mName(std::move(name)),
                mHasRetValue(true),
                mRetVal(std::move(ret)) {
        mArgs.emplace(std::move(arg1));
        mArgs.emplace(std::move(arg2));
        mArgs.emplace(std::move(arg3));
        mArgs.emplace(std::move(arg4));
    }

    void MethodInfo::showDebugInfo() const {
        printf("  Name: %s\n", getName().data());
        printf("  Signature: %s(", getRetAsString());
        for (uint32 i = 0; i < getArgsCount(); i++) {
            printf("%s %s", mArgs[i].getTypeAsString(), mArgs[i].getName().data());
            if (i + 1 != getArgsCount()) {
                printf(",");
            }
        }
        printf(")\n");
    }

}