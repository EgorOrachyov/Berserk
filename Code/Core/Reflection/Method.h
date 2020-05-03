/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_METHOD_H
#define BERSERK_METHOD_H

#include <Reflection/ReflectionFlags.h>
#include <Reflection/Variant.h>
#include <TEnumMask.h>

namespace Berserk {

    class Method {
    public:

        /** Generic signature of the an method */
        using GenericSignature = Function<EError(class Object& object, TArray<Variant> &args, Variant& result)>;

        Method(class Class& root, CString name, uint32 argsCount, bool retValue, EAccessMode accessMode, const TEnumMask<EAttributeOption> &options, GenericSignature& body);
        ~Method() = default;

        /**
         * Allows to call this method on the object
         * @param object Object to call method on
         * @param args Arguments to pass to the method
         * @param result Result of method if method returns something
         * @return Ok if method was successfully called
         */
        EError call(class Object& object, TArray<Variant> &args, Variant& result) const;

        /** @return Method name */
        const CString& getMethodName() const { return mMethodName; }

        /** @return Arguments count in the method */
        uint32 getArgumentsCount() const { return mArgumentsCount; }

        /** @return True if method has return value */
        bool getHasReturnValue() const { return mHasReturnValue; }

        /** @return Options of the method (for operations) */
        const TEnumMask<EAttributeOption> &getOptions() const { return mOptions; }

        /** @return Access mode in the source class */
        EAccessMode getAccessMode() const { return mAccessMode; }

    private:
        class Class* mClass = nullptr;
        CString mMethodName;
        uint32 mArgumentsCount = 0;
        bool mHasReturnValue = false;
        TEnumMask<EAttributeOption> mOptions;
        EAccessMode mAccessMode = EAccessMode::Private;
        GenericSignature mMethodBody;
    };

}

#endif //BERSERK_METHOD_H