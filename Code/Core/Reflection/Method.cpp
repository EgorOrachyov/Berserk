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

    EError Method::call(Object &object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant &ret) const {
        if (getArgsCount() != args.size()) {
            BERSERK_ERROR("Args count mismatched to call %s::%s", object.getClassName().data(), getName().data());
            return EError::FAILED_TO_CALL_METHOD;
        }

        // todo: append default args if needed
        TArrayStatic<Variant*,MethodInfo::MAX_ARGS> actualArgs;
        for (auto arg: args) {
            actualArgs.add(arg);
        }

        mCall(object, actualArgs, ret);

        return EError::OK;
    }

    void Method::showDebugInfo() const {
        mInfo.showDebugInfo();
    }

}