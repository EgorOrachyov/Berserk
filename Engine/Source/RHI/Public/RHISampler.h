//
// Created by Egor Orachyov on 2019-07-15.
//

#ifndef BERSERK_RHISAMPLER_H
#define BERSERK_RHISAMPLER_H

#include <RHIResource.h>
#include <RHIDefinitions.h>

namespace Berserk
{

    class GRAPHICS_API RHISampler : public RHIResource
    {
    public:

        virtual ~RHISampler() = default;

        /** @return Sampler minification filtering */
        virtual ESamplerFilter getMinFilter() const = 0;

        /** @return Sampler magnification filtering */
        virtual ESamplerFilter getMagFilter() const = 0;

        /** @return Sampler all-axis wrapping mode */
        virtual ESamplerWrapMode getWrapMode() const = 0;

        /** @return Border color for sampled texture */
        virtual const Vec4f& getBorderColor() const = 0;

    };

    typedef TSharedPtr<RHISampler> RHISamplerRef;

} // namespace Berserk

#endif //BERSERK_RHISAMPLER_H