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

    protected:

        CString mName;
    };

} // namespace Berserk

#endif //BERSERKENGINE_OBJECT_H