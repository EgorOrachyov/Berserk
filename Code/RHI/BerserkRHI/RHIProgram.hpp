/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIPROGRAM_HPP
#define BERSERK_RHIPROGRAM_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Memory/MemoryBuffer.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        class Program: public Resource {
        public:

            struct ShaderDesc {
                ShaderType type;
                Ref<MemoryBuffer> sourceCode;
            };

            struct Desc {
                StringName name;
                ShaderLanguage language;
                ArrayFixed<ShaderDesc> stages;
            };

            ~Program() override = default;

            /** @return Shader name for debugging */
            const StringName& GetShaderName() const { return mName; }

            /** @return Source code language type */
            ShaderLanguage GetLanguage() const { return mLanguage; }

            /** @return  Source code per stages */
            const ArrayFixed<ShaderDesc, Limits::MAX_SHADER_STAGES>& GetStages() const { return mStages; }

        protected:

            /** Shader name for debugging */
            StringName mName;

            /** Source code language type */
            ShaderLanguage mLanguage;

            /** Source code per stages */
            ArrayFixed<ShaderDesc, Limits::MAX_SHADER_STAGES> mStages;
        };

    }
}

#endif //BERSERK_RHIPROGRAM_HPP