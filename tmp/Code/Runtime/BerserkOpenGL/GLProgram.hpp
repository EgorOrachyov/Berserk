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

#ifndef BERSERK_GLPROGRAM_HPP
#define BERSERK_GLPROGRAM_HPP

#include <BerserkRHI/RHIProgram.hpp>
#include <BerserkCore/Platform/Synchronization.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLProgram: public Program {
        public:
            explicit GLProgram(const Desc& desc);
            ~GLProgram() override;

            void Initialize();
            bool ValidateStages() const;
            void CreateProgramMeta();
            void BindUniformBlock(uint32 binding) const;
            void Use() const;

            Status GetCompilationStatus() const override;
            String GetCompilerMessage() const override;
            RcPtr<ProgramMeta> GetProgramMeta() const override;

            GLuint GetHandle() const { return mHandle; }

        protected:
            String mCompilerMessage;
            RcPtr<ProgramMeta> mMeta;
            AtomicUint32 mCompilationStatus{(uint32) Status::PendingCompilation};
            GLuint mHandle = 0;
        };

    }
}

#endif //BERSERK_GLPROGRAM_HPP