/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CLASS_H
#define BERSERK_CLASS_H

#include <Reflection/Method.h>
#include <Reflection/Object.h>
#include <TRef.h>

namespace Berserk {

    /**
     * @brief Reflection structure
     *
     * Describes engine game objects and resources structure
     * for serialization and UI visualization.
     *
     * Allows to query in run-time inheritance structure,
     * properties of the objects and call its public methods.
     */
    class Class {
    public:

        const TArray<Method> &getMethods() const { return mClassMethods; }

    private:
        template <typename ClassType>
        friend class ClassBuilder;

        Class* mSuperclass = nullptr;
        CString mClassName;
        TArray<Method> mClassMethods;
    };

    template <typename ClassType>
    class ClassBuilder {
    public:

        template <typename T>
        void addMethodArgs0Ret(const char* name, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, T* method) {
            auto callable = [=](class Object& object, TArray<Variant> &args, Variant& result) -> EError {
                if (args.size() != 0) {
                    BERSERK_ERROR("Invalid number of arguments provided to the method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                auto ptr = dynamic_cast<ClassType*>(&object);

                if (ptr == nullptr) {
                    BERSERK_ERROR("Incompatible type of the object to call method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                result = (ptr->*method)();
                return EError::OK;
            };

            Method::GenericSignature body = callable;
            mBuildClass.mClassMethods.emplace(&mBuildClass, name, 0, true, accessMode, options, body);
        }

        template <typename T>
        void addMethodArgs1Ret(const char* name, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, T method) {
            auto callable = [=](class Object& object, TArray<Variant> &args, Variant& result) -> EError {
                if (args.size() != 1) {
                    BERSERK_ERROR("Invalid number of arguments provided to the method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                auto ptr = dynamic_cast<ClassType*>(&object);

                if (ptr == nullptr) {
                    BERSERK_ERROR("Incompatible type of the object to call method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                result = (ptr->*method)(args[0]);
                return EError::OK;
            };

            Method::GenericSignature body = callable;
            mBuildClass.mClassMethods.emplace(mBuildClass, name, 1, true, accessMode, options, body);
        }

        template <typename T>
        void addMethodArgs2Ret(const char* name, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, T* method) {
            auto callable = [=](class Object& object, TArray<Variant> &args, Variant& result) -> EError {
                if (args.size() != 2) {
                    BERSERK_ERROR("Invalid number of arguments provided to the method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                auto ptr = dynamic_cast<ClassType*>(&object);

                if (ptr == nullptr) {
                    BERSERK_ERROR("Incompatible type of the object to call method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                result = (ptr->*method)(args[0], args[1]);
                return EError::OK;
            };

            Method::GenericSignature body = callable;
            mBuildClass.mClassMethods.emplace(&mBuildClass, name, 1, true, accessMode, options, body);
        }

        template <typename T>
        void addMethodArgs3Ret(const char* name, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, T* method) {
            auto callable = [=](class Object& object, TArray<Variant> &args, Variant& result) -> EError {
                if (args.size() != 2) {
                    BERSERK_ERROR("Invalid number of arguments provided to the method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                auto ptr = dynamic_cast<ClassType*>(&object);

                if (ptr == nullptr) {
                    BERSERK_ERROR("Incompatible type of the object to call method");
                    return EError::FAILED_TO_CALL_METHOD;
                }

                result = (ptr->*method)(args[0], args[1], args[2]);
                return EError::OK;
            };

            Method::GenericSignature body = callable;
            mBuildClass.mClassMethods.emplace(&mBuildClass, name, 1, true, accessMode, options, body);
        }

        /** @return Built class */
        const Class& getClass() const { return mBuildClass; }

    private:

        Class mBuildClass;
    };

}

#endif //BERSERK_CLASS_H