//
// Created by Egor Orachyov on 25.07.2018.
//

#ifndef BERSERKENGINE_OBJECT_H
#define BERSERKENGINE_OBJECT_H

#include "Strings/CString.h"
#include "Strings/CStaticString.h"

namespace Berserk
{

    class Object
    {
    public:

        Object(const CStaticString& name);
        virtual ~Object();

        const CString& getName() const;

        bool isStatic() const;

    protected:

        friend class ObjectManager;

        INT8 mIsStatic      : 1;
        INT8 mIsActive      : 1;
        INT8 mIsVisible     : 1;
        INT8 mIsPaused      : 1;
        INT8 mIsEditable    : 1;
        INT8 mIsAttachable  : 1;
        INT8 mCastShadows   : 1;
        INT8 mIsToggleable  : 1;
        INT8 mIsDamageable  : 1;
        INT8 data2;
        INT8 data3;

        CString mName;
    };

} // namespace Berserk

#endif //BERSERKENGINE_OBJECT_H