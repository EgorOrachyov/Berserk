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

#ifndef BERSERK_RHIPROGRAM_HPP
#define BERSERK_RHIPROGRAM_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Strings/String.hpp>
#include <BerserkCore/Templates/MemoryBuffer.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        class Program: public Resource {
        public:

            struct ShaderDesc {
                ShaderType type;
                RefCounted<ReadOnlyMemoryBuffer> sourceCode;
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