//
// Created by Egor Orachyov on 25.07.2018.
//

#include "Objects/Object.h"

namespace Berserk
{

    Object::Object(const CStaticString& name)
    {
        mName.copy(name);
        mIsStatic = true;
    }

    Object::~Object()
    {
        /// Add something useful
    }

    const CString& Object::getName() const
    {
        return mName;
    }

    bool Object::isStatic() const
    {
        return mIsStatic;
    }

} // namespace Berserk