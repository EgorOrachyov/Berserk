/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/Object.h>
#include <Reflection/Method.h>
#include <Reflection/ClassBuilder.h>
#include <ErrorMacro.h>

namespace Berserk {

    class Class& Object::getClass() const {
        return getClassStatic();
    }

    class Class& Object::getClassStatic() {
        return ClassManager::getSingleton().getClass(getClassNameStatic());
    }

    EError Object::call(const CString &methodName, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant &ret) {
        Class& thisClass = getClass();

        if (thisClass.hasMethod(methodName)) {
            const Method& method = thisClass.getMethod(methodName);
            return method.call(*this, args, ret);
        }

        BERSERK_ERROR_RET_VALUE(EError::FAILED_TO_CALL_METHOD, "No such method %s::%s", thisClass.getClassName().data(), methodName.data());
    }

    void Object::registerInfo() {
        ClassBuilder<Object> builder;

        builder
            .registerClass()
            .addMethod(EVariantType::String, "getClassName", &Object::getClassName)
            .addMethod(EVariantType::String, "getSuperClassName", &Object::getSuperClassName)
            .addMethod(EVariantType::Bool, "hasSuperClass", &Object::hasSuperClass);
    }

}
