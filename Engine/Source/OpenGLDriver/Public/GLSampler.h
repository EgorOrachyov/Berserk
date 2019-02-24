//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_GLSAMPLER_H
#define BERSERK_GLSAMPLER_H

#include "Platform/ISampler.h"

namespace Berserk
{

    class GLSampler : public ISampler
    {
    public:

        ~GLSampler() = default;

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
        virtual void create(IRenderDriver::SamplerFilter min,
                            IRenderDriver::SamplerFilter max,
                            IRenderDriver::SamplerWrapMode wrap) = 0;

        /** @copydoc ISampler::setFiltering() */
        virtual void setFiltering(IRenderDriver::SamplerFilter min,
                                  IRenderDriver::SamplerFilter max) = 0;

        /** @copydoc ISampler::setWrapping() */
        virtual void setWrapping(IRenderDriver::SamplerWrapMode wrap) = 0;

        /** @copydoc ISampler::setBorderColor() */
        virtual void setBorderColor(const Vec4f& color) = 0;

    };

} // namespace Berserk

#endif //BERSERK_GLSAMPLER_H