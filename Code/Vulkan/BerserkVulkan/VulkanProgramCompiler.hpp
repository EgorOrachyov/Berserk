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

#ifndef BERSERK_VULKANPROGRAMCOMPILER_HPP
#define BERSERK_VULKANPROGRAMCOMPILER_HPP

#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkVulkan/VulkanDefs.hpp>
#include <BerserkCore/Templates/SmartPointer.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * @brief Vulkan program compiler
         *
         * Creates vulkan programs, asynchronously compiles glsl sources
         * into spir-v and asynchronously generates reflection info.
         *
         * Uses system thread pool for jobs scheduling.
         */
        class VulkanProgramCompiler {
        public:
            explicit VulkanProgramCompiler(class VulkanDevice& device);
            ~VulkanProgramCompiler();

            /**
             * Data associated with each program for its compilation.
             * Compiler outputs here compiled binaries,
             * status and error message if present,
             * also generates reflection info for the program.
             */
            struct ProgramCompileData {
                ArrayFixed<RefCounted<ReadOnlyMemoryBuffer>, Limits::MAX_SHADER_STAGES> binaries;
                RefCounted<class VulkanProgram> program;
                RefCounted<ProgramMeta> meta;
                String message;
                bool compiled = false;
            };

            /**
             * Create and compile vulkan shader program.
             * This function creates program object and returns immediately.
             *
             * Compilation process is asynchronous and finishes as soon
             * as glsl sources are compiled into spir-v binaries.

             * @return Vulkan Program object
             */
            RefCounted<Program> CreateProgram(const Program::Desc &desc);


            /** Called each frame to initialize pending create programs */
            void Update();

        private:
            void Compile(ProgramCompileData& compileData) const;

        private:
            mutable Mutex mutex;
            // todo: add async tasks here
            Array<SharedPtr<ProgramCompileData>> mPendingCreate;
            Array<SharedPtr<ProgramCompileData>> mCachedBuffer;
            class VulkanDevice& mDevice;
        };

    }
}

#endif //BERSERK_VULKANPROGRAMCOMPILER_HPP