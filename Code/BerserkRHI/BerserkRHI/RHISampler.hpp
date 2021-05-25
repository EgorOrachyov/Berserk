/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
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