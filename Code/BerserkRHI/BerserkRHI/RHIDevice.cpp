/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkRHI/RHIDevice.hpp>

namespace Berserk {
    namespace RHI {

        const Array<TextureFormat> & Device::GetSupportedFormats() const {
            return mSupportedTextureFormats;
        }

        const Array<ShaderLanguage> & Device::GetSupportedShaderLanguages() const {
            return mSupportedShaderLanguages;
        }

    }
}
