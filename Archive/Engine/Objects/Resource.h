//
// Created by Egor Orachyov on 28.07.2018.
//

#ifndef BERSERKENGINE_RESOURCE_H
#define BERSERKENGINE_RESOURCE_H

#include "Objects/Object.h"

namespace Berserk
{

    class Resource : public Object
    {
    public:

        Resource(const CStaticString& name) : Object(name) {};
        virtual ~Resource() = default;

        virtual void load(const CString &fullFileName) = 0;
        virtual void load(const CString &fileName, const CString &path) = 0;
        virtual void use() = 0;

        virtual bool isLoaded() const = 0;
        virtual const CString &getResourceFileName() const;

    protected:

        CString mResourceName;

    };

} // namespace Berserk

#endif //BERSERKENGINE_RESOURCE_H
