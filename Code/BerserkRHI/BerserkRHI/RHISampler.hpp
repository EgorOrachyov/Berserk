/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHISAMPLER_HPP
#define BERSERK_RHISAMPLER_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>

namespace Berserk {
    namespace RHI {

        class Sampler: public Resource {
        public:

            struct Desc {
                float minLod;
                float maxLod;
                SamplerMinFilter minFilter : 4;
                SamplerMagFilter magFilter : 4;
                SamplerRepeatMode u : 4;
                SamplerRepeatMode v : 4;
                SamplerRepeatMode w : 4;
                SamplerBorderColor color : 4;

                Desc() {
                    minLod = 0;
                    maxLod = Limits::MAX_TEXTURE_MIPMAPS;
                    minFilter = SamplerMinFilter::Nearest;
                    magFilter = SamplerMagFilter::Nearest;
                    u = SamplerRepeatMode::Repeat;
                    v = SamplerRepeatMode::Repeat;
                    w = SamplerRepeatMode::Repeat;
                    color = SamplerBorderColor::Black;
                }
            };

            ~Sampler() override = default;

            /** @return Sampler desc state */
            const Desc& GetDesc() const { return mState; }

        protected:
            /** Sampler desc state */
            Desc mState;
        };

    }
}

#endif //BERSERK_RHISAMPLER_HPP