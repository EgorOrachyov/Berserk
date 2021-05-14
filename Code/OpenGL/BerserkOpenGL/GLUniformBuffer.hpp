/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLUNIFORMBUFFER_HPP
#define BERSERK_GLUNIFORMBUFFER_HPP

#include <BerserkRHI/RHIUniformBuffer.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLUniformBuffer : public UniformBuffer {
        public:
            explicit GLUniformBuffer(const Desc &desc);
            ~GLUniformBuffer() override;

            void Initialize(const Ref<MemoryBuffer> &buffer);
            void Update(uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory);

            GLuint GetHandle() const {
                return mHandle;
            }

        protected:
            void OnReleased() const override;

            GLuint mHandle = 0;
        };

    }
}

#endif //BERSERK_GLUNIFORMBUFFER_HPP
