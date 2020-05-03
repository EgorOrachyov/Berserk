/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PARAM_H
#define BERSERK_PARAM_H

#include <Reflection/ReflectionFlags.h>
#include <Reflection/Variant.h>
#include <TEnumMask.h>

namespace Berserk {

    class Param {
    public:

        ~Param() = default;

    private:

        class Class* mClass = nullptr;
        CString mParamName;
        EAccessMode mAccessMode;
        TEnumMask<EAttributeOption> mOptions;
        uint32 mSize;
        uint32 mOffset;

    };

}

#endif //BERSERK_PARAM_H
