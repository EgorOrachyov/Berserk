/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CLASSBUILDER_H
#define BERSERK_CLASSBUILDER_H

#include <Reflection/Class.h>
#include <Reflection/ClassManager.h>
#include <TPtrShared.h>

namespace Berserk {

    template <typename T>
    class ClassBuilder {
    public:

        ClassBuilder& registerClass() {
            CString className = T::getClassNameStatic();

            if (ClassManager::getSingleton().hasClass(className)) {
                BERSERK_ERROR("Class %s already created an registered", className.data());
                return *this;
            }

            TPtrShared<Class> instance = TPtrShared<Class>::make();
            instance->mClassName = className;

            if (T::hasSuperClassStatic()) {
                instance->mHasSuperClass = true;
                instance->mSuperClassName = T::getSuperClassNameStatic();
            }

            ClassManager::getSingleton().registerClass(instance);
            mClass = instance.getPtr();

            return *this;
        }

        ClassBuilder& addProperty(CString name, CString className, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            if (mClass->hasProperty(name)) {
                BERSERK_ERROR("Class %s already has property %s", mClass->getClassName().data(), name.data());
                return *this;
            }

            CString key = name;
            Property p(std::move(name),std::move(className),flags,std::move(hint));
            mClass->mProperties.move(key, p);

            return *this;
        }

        ClassBuilder& addProperty(CString name, EVariantType type, TEnumMask<EPropertyFlags> flags = {}, CString hint = "") {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            if (mClass->hasProperty(name)) {
                BERSERK_ERROR("Class already has property %s::%s", mClass->getClassName().data(), name.data());
                return *this;
            }

            CString key = name;
            Property p(std::move(name),type,flags,std::move(hint));
            mClass->mProperties.move(key, p);

            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(Property ret, CString name, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallRetNoArgs(callable, call);
            MethodInfo methodInfo(std::move(ret), std::move(name));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(Property ret, CString name, Property arg1, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallRetArgs1(callable, call);
            MethodInfo methodInfo(std::move(ret), std::move(name), std::move(arg1));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(Property ret, CString name, Property arg1, Property arg2, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallRetArgs2(callable, call);
            MethodInfo methodInfo(std::move(ret), std::move(name), std::move(arg1), std::move(arg2));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(Property ret, CString name, Property arg1, Property arg2, Property arg3, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallRetArgs3(callable, call);
            MethodInfo methodInfo(std::move(ret), std::move(name), std::move(arg1), std::move(arg2), std::move(arg3));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(Property ret, CString name, Property arg1, Property arg2, Property arg3, Property arg4, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallRetArgs4(callable, call);
            MethodInfo methodInfo(std::move(ret), std::move(name), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(CString name, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallNoRetNoArgs(callable, call);
            MethodInfo methodInfo(std::move(name));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(CString name, Property arg1, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallNoRetArgs1(callable, call);
            MethodInfo methodInfo(std::move(name), std::move(arg1));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(CString name, Property arg1, Property arg2, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallNoRetArgs2(callable, call);
            MethodInfo methodInfo(std::move(name), std::move(arg1), std::move(arg2));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(CString name, Property arg1, Property arg2, Property arg3, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallNoRetArgs3(callable, call);
            MethodInfo methodInfo(std::move(name), std::move(arg1), std::move(arg2), std::move(arg3));

            addMethodInternal(methodInfo, call);
            return *this;
        }

        template <typename M>
        ClassBuilder& addMethod(CString name, Property arg1, Property arg2, Property arg3, Property arg4, M callable) {
            BERSERK_COND_ERROR_RET_VALUE(*this, mClass, "Class is not initialized");

            Function<Method::Call> call;
            getCallNoRetArgs4(callable, call);
            MethodInfo methodInfo(std::move(name), std::move(arg1), std::move(arg2), std::move(arg3), std::move(arg4));

            addMethodInternal(methodInfo, call);
            return *this;
        }

    private:

        void addMethodInternal(MethodInfo& methodInfo, Function<Method::Call> &call) {
            auto& name = methodInfo.getName();

            if (mClass->hasMethod(name)) {
                BERSERK_ERROR("Class already has method %s::%s", mClass->getClassName().data(), name.data());
                return;
            }

            CString key = name;
            Method method;
            method.mInfo = std::move(methodInfo);
            method.mCall = std::move(call);
            mClass->mMethods.move(key, method);
        }

        template <typename M>
        void getCallNoRetNoArgs(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                (*t.*callable)();
            };

            call = internal;
        }

        template <typename M>
        void getCallNoRetArgs1(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                (*t.*callable)(*args[0]);
            };

            call = internal;
        }

        template <typename M>
        void getCallNoRetArgs2(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                (*t.*callable)(*args[0], *args[1]);
            };

            call = internal;
        }

        template <typename M>
        void getCallNoRetArgs3(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                (*t.*callable)(*args[0], *args[1], *args[2]);
            };

            call = internal;
        }

        template <typename M>
        void getCallNoRetArgs4(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                (*t.*callable)(*args[0], *args[1], *args[2], *args[3]);
            };

            call = internal;
        }

        template <typename M>
        void getCallRetNoArgs(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                ret = (*t.*callable)();
            };

            call = internal;
        }

        template <typename M>
        void getCallRetArgs1(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                ret = (*t.*callable)(*args[0]);
            };

            call = internal;
        }

        template <typename M>
        void getCallRetArgs2(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                ret = (*t.*callable)(*args[0], *args[1]);
            };

            call = internal;
        }

        template <typename M>
        void getCallRetArgs3(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                ret = (*t.*callable)(*args[0], *args[1], *args[2]);
            };

            call = internal;
        }

        template <typename M>
        void getCallRetArgs4(M callable, Function<Method::Call> &call) {
            auto internal = [=](class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) {
                T* t = (T*) &object;
                ret = (*t.*callable)(*args[0], *args[1], *args[2], *args[3]);
            };

            call = internal;
        }

        Class* mClass = nullptr;

    };

}

#endif //BERSERK_CLASSBUILDER_H