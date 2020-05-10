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
#include <Reflection/Field.h>
#include <TRef.h>
#include <UUID.h>

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
    class Class final {
    public:

        /**
         * Collect inherited classes by this class
         * @param classes Array to store inherited class (Empty if this class is super class)
         */
        void inheritedClasses(TArray<Class*> &classes);

        /**
         * Checks whether this class has predecessor in its inheritance three.
         * @param predecessor Class to check if this class inherits
         * @note This class isn not subclass of this class
         * @return True if this class is subclass of predecessor
         */
        bool isSubclassOf(Class& predecessor);

        /** @return True if has method of specified name (case-sensitive) */
        bool hasMethod(const char* method) const;

        /** @return True if has field of specified name (case-sensitive) */
        bool hasField(const char* field) const;

        /** @return Super class for this class (null for root class) */
        TRef<Class> getSuperClass() const { return mSuperclass; }

        /** @return Externally created class UUID (stays the same among app calls) */
        const UUID &getClassUUID() const { return mID; }

        /** @return Class name */
        const CString &getClassName() const { return mClassName; }

        /** @return Reflected class methods */
        const TArray<Method> &getMethods() const { return mClassMethods; }

        /** @return Reflected class fields */
        const TArray<Field> &getFields() const { return mClassFields; }

    private:
        template <typename ClassType>
        friend class ClassBuilder;

        UUID mID;
        Class* mSuperclass = nullptr;
        CString mClassName;
        TArray<Method> mClassMethods;
        TArray<Field> mClassFields;
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