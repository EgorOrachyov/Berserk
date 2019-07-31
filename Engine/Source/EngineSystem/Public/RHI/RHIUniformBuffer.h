//
// Created by Egor Orachyov on 2019-07-11.
//

#ifndef BERSERK_RHIUNIFORMBUFFER_H
#define BERSERK_RHIUNIFORMBUFFER_H

#include <RHI/RHIResource.h>

namespace Berserk
{

    class GRAPHICS_API RHIUniformBuffer : public RHIResource
    {
    public:

        virtual ~RHIUniformBuffer() = default;

        /** Update uniform buffer with data */
        virtual void update(uint32 size, const uint8 *data) = 0;

        /** Update uniform buffer only after chosen offset */
        virtual void update(uint32 size, uint32 offset, const uint8 *data) = 0;

        /** Bind uniform buffer to chosen binding point */
        virtual void bind(uint32 bindingPoint) const = 0;

        /** Bind uniform buffer to its point */
        virtual void bind() const = 0;

        /** Set fixed binding point for uniform buffer */
        virtual void setBindingPoint(uint32 bindingPoint) = 0;

        /** @return Uniform buffer binding point to shader */
        virtual uint32 getBindingPoint() const = 0;

        /** @return Size of uniform buffer data */
        virtual uint32 getSize() const = 0;

    };

    typedef TSharedPtr<RHIUniformBuffer> RHIUniformBufferRef;

} // namespace Berserk

#endif //BERSERK_RHIUNIFORMBUFFER_H