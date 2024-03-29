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

#ifndef BERSERK_VULKANPROGRAM_HPP
#define BERSERK_VULKANPROGRAM_HPP

#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>

namespace Berserk {
    namespace RHI {

        /** Wrapper for vulkan compiled spir-v modules. Modules are compiled externally by ProgramManager. */
        class VulkanProgram final: public Program {
        public:
            VulkanProgram(class VulkanDevice& device, const Desc& desc);
            ~VulkanProgram() override;

            /**
             * Initialize program from compiled spir-v binaries, provided by program compiler.
             * This function is called only after successful compilation.
             *
             * @param modules Compiled modules byte-code
             * @param meta Reflected meta information about the program
             */
            void InitializedFromBinary(const ArrayFixed<RcPtr<Data>, Limits::MAX_SHADER_STAGES> &modules, RcPtr<ProgramMeta> meta);

            /**
             * Notify program that failed to compile glsl into spir-v binaries.
             * This function is called only after failed compilation.
             *
             * @param message Compiler error message
             */
            void NotifyFailedCompile(String message);

            /** @see Program::GetCompilationStatus() */
            Status GetCompilationStatus() const override;

            /** @see Program::GetCompilerMessage() */
            String GetCompilerMessage() const override;

            /** @see Program::GetProgramMeta() */
            RcPtr<ProgramMeta> GetProgramMeta() const override;

            /** @return Vulkan created modules for each program stage */
            const ArrayFixed<VkShaderModule, Limits::MAX_SHADER_STAGES> &GetModules() const { return mModules; };

        private:
            ArrayFixed<VkShaderModule, Limits::MAX_SHADER_STAGES> mModules;
            String mCompilerMessage;
            RcPtr<ProgramMeta> mMeta;
            AtomicUint32 mStatus{(uint32) Status::PendingCompilation};

            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANPROGRAM_HPP