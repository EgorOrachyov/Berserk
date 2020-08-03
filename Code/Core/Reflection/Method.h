/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_METHOD_H
#define BERSERK_METHOD_H

#include <Reflection/Property.h>
#include <Reflection/MethodInfo.h>

namespace Berserk {

    enum class EMethodFlags {
        Virtual = 0,
        HideFromEditor
    };

    class Method {
    public:

        /** Generic signature used to call method on objects */
        using Call = void(class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret);

        /** @return True if has return value */
        bool hasRetValue() const { return mInfo.hasRetValue(); }

        /** @return True if has args */
        bool hasArgs() const { return mInfo.getArgsCount() > 0; }

        /** @return Args count */
        uint32 getArgsCount() const { return mInfo.getArgsCount(); }

        /** @return Method name */
        const CString& getName() const { return mInfo.getName(); }

        /** @return Return value */
        const Property& getRetValue() const { return mInfo.getRetValue(); }

        /** @return Arguments */
        const TArrayStatic<Property,MethodInfo::MAX_ARGS> &getArgs() const { return mInfo.getArgs(); }

        /** @return Method info */
        const MethodInfo& getMethodInfo() const { return mInfo; }

        /** @return Method flags */
        TEnumMask<EMethodFlags> getFlags() const { return mFlags; }

        /**
         * Calls method for the provided object
         * @param object Object reference to call method for
         * @param args Arguments to pass to the method
         * @param ret Optional return value to store (if method has return value)
         * @return Status of the method call
         */
        EError call(class Object& object, const TArrayStatic<Variant*,MethodInfo::MAX_ARGS> &args, Variant& ret) const;

        /** Print debug info */
        void showDebugInfo() const;

    private:
        template <typename T>
        friend class ClassBuilder;

        MethodInfo mInfo;
        TEnumMask<EMethodFlags> mFlags;
        TArrayStatic<Variant,MethodInfo::MAX_ARGS> mDefArgs;
        Function<Call> mCall;
    };

}

#endif //BERSERK_METHOD_H