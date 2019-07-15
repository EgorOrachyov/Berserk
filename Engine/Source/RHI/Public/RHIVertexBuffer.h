//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIVERTEXBUFFER_H
#define BERSERK_RHIVERTEXBUFFER_H

#include <RHIResource.h>

namespace Berserk
{

    class GRAPHICS_API RHIVertexBuffer : public RHIResource
    {
    public:

        virtual ~RHIVertexBuffer() = default;

        /** Updates buffer content */
        virtual void update(uint32 size, const uint8* data) = 0;

        /** @return Buffer data size */
        virtual uint32 getSize() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_RHIVERTEXBUFFER_H