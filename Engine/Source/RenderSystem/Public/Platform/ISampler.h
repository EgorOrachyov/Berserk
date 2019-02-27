//
// Created by Egor Orachyov on 07.02.2019.
//

#ifndef BERSERK_ISAMPLER_H
#define BERSERK_ISAMPLER_H

#include "Misc/Bits.h"
#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Resource/IResource.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    class GRAPHICS_API ISampler : public IResource
    {
    public:

        /**
         * Creates sampler with chosen params
         * @param min Minification filtering
         * @param mag Magnification filtering
         * @param wrap Wrapping mod for texture
         */
        virtual void create(IRenderDriver::SamplerFilter min,
                            IRenderDriver::SamplerFilter mag,
                            IRenderDriver::SamplerWrapMode wrap) = 0;

        /** Bind sampler ito chosen texture slot */
        virtual void bind(uint32 textureSlot) = 0;

        /**
         * Set filtering params
         * @param min Minification filtering
         * @param mag Magnification filtering
         */
        virtual void setFiltering(IRenderDriver::SamplerFilter min,
                                  IRenderDriver::SamplerFilter mag) = 0;

        /**
         * Set wrapping mode
         * @param wrap Wrapping mod for texture
         */
        virtual void setWrapping(IRenderDriver::SamplerWrapMode wrap) = 0;

        /**
         * Enable border color using for texture
         * @param color Chosen border color
         */
        virtual void setBorderColor(const Vec4f& color) = 0;

        /** @return Memory used at gpu side */
        virtual uint32 getGPUMemoryUsage() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ISAMPLER_H