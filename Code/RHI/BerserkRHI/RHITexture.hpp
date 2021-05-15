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

#ifndef BERSERK_RHITEXTURE_HPP
#define BERSERK_RHITEXTURE_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Templates/Mask.hpp>
#include <BerserkCore/Math/TVecN.hpp>

namespace Berserk {
    namespace RHI {

        class Texture: public Resource {
        public:

            struct Desc {
                uint32 width;
                uint32 height;
                uint32 depth;
                uint32 mipsCount;
                uint32 arraySlices;
                TextureType textureType;
                TextureFormat textureFormat;
                Mask<TextureUsage> textureUsage;
            };

            ~Texture() override = default;

            uint32 GetWidth() const { return mDesc.width; }
            uint32 GetHeight() const { return mDesc.height; }
            uint32 GetDepth() const { return mDesc.depth; }
            uint32 GetMipsCount() const { return mDesc.mipsCount; }
            uint32 GetArraySlices() const { return mDesc.arraySlices; }

            /** @return Texture desc */
            const Desc& GetDesc() const { return mDesc; }

        private:

            /** Texture desc */
            Desc mDesc;
        };

    }
}

#endif //BERSERK_RHITEXTURE_HPP