/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/Method.h>
#include <Reflection/Object.h>

namespace Berserk {

    Method::Method(class Class& root, CString name, uint32 argsCount, bool retValue, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, GenericSignature &body) {
        mClass = &root;
        mMethodName = std::move(name);
        mArgumentsCount = argsCount;
        mHasReturnValue = retValue;
        mOptions = options;
        mAccessMode = accessMode;
        mMethodBody = std::move(body);
    }

    EError Method::call(struct Object &object, TArray<Variant> &args, Variant &result) const {
        return mMethodBody(object, args, result);
    }

}