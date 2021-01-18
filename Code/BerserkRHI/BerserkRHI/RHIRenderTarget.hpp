/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIRENDERTARGET_HPP
#define BERSERK_RHIRENDERTARGET_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkRHI/RHITexture.hpp>
#include <BerserkCore/Containers/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        class RenderTarget: public Resource {
        public:

            /** Describes single render target attachment */
            struct AttachmentDesc {
                Ref<Texture> target;
                uint32 face = 0;
                uint32 mipLevel = 0;
            };

            struct Desc {
                uint32 mWidth;
                uint32 mHeight;
                AttachmentDesc mDepthStencilTarget;
                ArrayFixed<AttachmentDesc> mColorTargets;
            };

            ~RenderTarget() override = default;

            /** @return Render target desc */
            const Desc& GetDesc() const { return mDesc; }

        private:

            /** Render target desc */
            Desc mDesc;
        };

    }
}

#endif //BERSERK_RHIRENDERTARGET_HPP