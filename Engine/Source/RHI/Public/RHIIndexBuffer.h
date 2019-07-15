//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIINDEXBUFFER_H
#define BERSERK_RHIINDEXBUFFER_H

#include <RHIResource.h>
#include <RHIDefinitions.h>

namespace Berserk
{

    class RHIIndexBuffer : public RHIResource
    {
    public:

        virtual ~RHIIndexBuffer() = default;

        /** Updates buffer content */
        virtual void update(uint32 size, const uint8* data) = 0;

        /** @return Buffer data size */
        virtual uint32 getSize() const = 0;

        /** @return Type of indices in this buffer */
        virtual EIndexType getIndexType() const  = 0;

    };

} // namespace Berserk

#endif //BERSERK_RHIINDEXBUFFER_H