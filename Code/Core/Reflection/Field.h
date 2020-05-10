/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FIELD_H
#define BERSERK_FIELD_H

#include <Reflection/ReflectionFlags.h>
#include <Reflection/Variant.h>
#include <TEnumMask.h>

namespace Berserk {

    class Field {
    public:

        ~Field() = default;

        const CString& getFieldName() const { return mFieldName; }

    private:

        class Class* mClass = nullptr;
        CString mFieldName;
        EAccessMode mAccessMode;
        TEnumMask<EAttributeOption> mOptions;
        uint32 mSize;
        uint32 mOffset;

    };

}

#endif //BERSERK_FIELD_H
