//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_RENDERRESOURCE_H
#define BERSERKENGINE_RENDERRESOURCE_H

#include "Essential/Types.h"
#include "Strings/CStaticString.h"

namespace Berserk
{
    class RenderResource
    {
    public:

        virtual ~RenderResource() = default;

        virtual UINT32 getMemoryUsage() const = 0;

        virtual void   addReference() = 0;
        virtual void   release() = 0;
        virtual UINT32 getReferences() const = 0;

        virtual void setName(const CStaticString& name) = 0;
        virtual const CStaticString& getName() const = 0;
    };
}

#endif //BERSERKENGINE_RENDERRESOURCE_H