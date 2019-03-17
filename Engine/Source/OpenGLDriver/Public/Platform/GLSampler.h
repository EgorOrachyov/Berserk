//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLSAMPLER_H
#define BERSERK_GLSAMPLER_H

#include "Platform/ISampler.h"
#include "Strings/String.h"

namespace Berserk
{

    /**
     * OpenGL platform sampler implementation
     */
    class GRAPHICS_API GLSampler : public ISampler
    {
    public:

        ~GLSampler();

        /** @copydoc IResource::initialize() */
        void initialize(const char* name) override;

        /** @copydoc IResource::addReference() */
        void addReference() override;

        /** @copydoc IResource::release() */
        void release() override;

        /** @copydoc IResource::getReferenceCount() */
        uint32 getReferenceCount() override;

        /** @copydoc IResource::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IResource::getName() */
        const char* getName() override;

    public:

        /** @copydoc ISampler::create() */
        void create(IRenderDriver::SamplerFilter min,
                    IRenderDriver::SamplerFilter mag,
                    IRenderDriver::SamplerWrapMode wrap) override;

        /** @copydoc ISampler::bind() */
        void bind(uint32 textureSlot) override;

        /** @copydoc ISampler::setFiltering() */
        void setFiltering(IRenderDriver::SamplerFilter min,
                          IRenderDriver::SamplerFilter mag) override;

        /** @copydoc ISampler::setWrapping() */
        void setWrapping(IRenderDriver::SamplerWrapMode wrap) override;

        /** @copydoc ISampler::setBorderColor() */
        void setBorderColor(const Vec4f& color) override;

        /** @copydoc ITexture::getGPUMemoryUsage() */
        uint32 getGPUMemoryUsage() override;

    private:

        friend class GLTextureManager;

        uint32 mReferenceCount;
        uint32 mSamplerID;
        uint32 mFilteringMin;
        uint32 mFilteringMag;
        uint32 mWrapping;
        Vec4f  mBorderColor;
        CString mResourceName;

    };

} // namespace Berserk

#endif //BERSERK_GLSAMPLER_H