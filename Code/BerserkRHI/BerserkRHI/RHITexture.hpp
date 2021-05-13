/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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