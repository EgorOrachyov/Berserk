//
// Created by Egor Orachyov on 27.07.2018.
//

#ifndef BERSERKENGINE_GPUBUFFER_H
#define BERSERKENGINE_GPUBUFFER_H

#include "Objects/Object.h"

namespace Berserk
{

    class GPUBuffer : public Object
    {
    public:

        GPUBuffer(const CStaticString &name) : Object(name) {}
        virtual ~GPUBuffer() = default;

        virtual void init() = 0;
        virtual void destroy() = 0;

        virtual void drawData() const = 0;
        virtual void drawIndices() const = 0;

        virtual bool validate() const = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_GPUBUFFER_H