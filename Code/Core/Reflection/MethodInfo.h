/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_METHODINFO_H
#define BERSERK_METHODINFO_H

#include <Reflection/Property.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {

    class MethodInfo {
    public:

        /** Max number of the arguments to pass to the method */
        static const uint32 MAX_ARGS = 8;

        MethodInfo() = default;

        MethodInfo(CString name);
        MethodInfo(CString name, Property arg1);
        MethodInfo(CString name, Property arg1, Property arg2);
        MethodInfo(CString name, Property arg1, Property arg2, Property arg3);
        MethodInfo(CString name, Property arg1, Property arg2, Property arg3, Property arg4);

        MethodInfo(Property ret, CString name);
        MethodInfo(Property ret, CString name, Property arg1);
        MethodInfo(Property ret, CString name, Property arg1, Property arg2);
        MethodInfo(Property ret, CString name, Property arg1, Property arg2, Property arg3);
        MethodInfo(Property ret, CString name, Property arg1, Property arg2, Property arg3, Property arg4);

        /** @return True if has return value */
        bool hasRetValue() const { return mHasRetValue; }

        /** @return True if has args */
        bool hasArgs() const { return getArgsCount() > 0; }

        /** @return Args count */
        uint32 getArgsCount() const { return mArgs.size(); }

        /** @return Method name */
        const CString& getName() const { return mName; }

        /** @return Return value */
        const Property& getRetValue() const { return mRetVal; }

        /** @return Arguments */
        const TArrayStatic<Property,MAX_ARGS> &getArgs() const { return mArgs; }

        /** @return Return type as string for print output */
        const char* getRetAsString() const { return hasRetValue()? mRetVal.getTypeAsString(): "void"; }

        /** Print debug method info */
        void showDebugInfo() const;

    private:

        CString mName;
        bool mHasRetValue = false;
        Property mRetVal;
        TArrayStatic<Property,MAX_ARGS> mArgs;

    };

}

#endif //BERSERK_METHODINFO_H