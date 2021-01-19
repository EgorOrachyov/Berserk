/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkRHI/RHIDriver.hpp>

namespace Berserk {
    namespace RHI {

        const Array<TextureFormat> & Driver::GetSupportedFormats() const {
            return mSupportedTextureFormats;
        }

        const Array<ShaderLanguage> & Driver::GetSupportedShaderLanguages() const {
            return mSupportedShaderLanguages;
        }

    }
}