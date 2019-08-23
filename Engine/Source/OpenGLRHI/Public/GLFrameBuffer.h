//
// Created by Egor Orachyov on 2019-08-23.
//

#ifndef BERSERK_GLFRAMEBUFFER_H
#define BERSERK_GLFRAMEBUFFER_H

namespace Berserk
{

    class GLFrameBufferTarget final : public RHIFrameBuffer
    {
    public:

        GLFrameBufferTarget(const RHITexture2DRef &color, const RHITexture2DRef &depth, GLenum gl_depthAttachment);

        ~GLFrameBufferTarget() override;

        void bind() override;

        uint32 getColorAttachmentsCount() const override;

        RHITextureRef getDepthAttachment() const override;

        RHITextureRef getColorAttachment(uint32 layer) const override;

        uint32 getMemoryUsage() const override;

        uint32 getMemoryUsage_GPU() const override;

    private:

        GLuint mResourceID = 0;
        RHITexture2DRef mColorBuffer;
        RHITexture2DRef mDepthStencil;

    };

} // namespace Berserk

#endif //BERSERK_GLFRAMEBUFFER_H
