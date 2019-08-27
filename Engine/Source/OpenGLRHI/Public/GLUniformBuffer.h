//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLUNIFORMBUFFER_H
#define BERSERK_GLUNIFORMBUFFER_H

#include <RHI/RHIUniformBuffer.h>
#include <GLCommon.h>

namespace Berserk
{

    class GLUniformBuffer final : public RHIUniformBuffer
    {
    public:

        GLUniformBuffer(uint32 bindingPoint, GLenum bufferUsage,
                        uint32 size, const uint8* data);

        ~GLUniformBuffer() override;

        void update(uint32 size, const uint8 *data) override;

        void update(uint32 size, uint32 offset, const uint8 *data) override;

        void bind(uint32 bindingPoint) const override;

        void bind() const override;

        void setBindingPoint(uint32 bindingPoint) override;

        uint32 getBindingPoint() const override;

        uint32 getSize() const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        GLuint mResourceID = 0;
        uint32 mBufferSize = 0;
        uint32 mBindingPoint = 0;

    };

} // namespace Berserk

#endif //BERSERK_GLUNIFORMBUFFER_H
