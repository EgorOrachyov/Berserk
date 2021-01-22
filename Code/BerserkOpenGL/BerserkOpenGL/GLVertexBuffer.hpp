/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLVERTEXBUFFER_HPP
#define BERSERK_GLVERTEXBUFFER_HPP

#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLVertexBuffer : public VertexBuffer {
        public:
            explicit GLVertexBuffer(const Desc &desc);
            ~GLVertexBuffer() override;

            void Initialize(const Ref<MemoryBuffer> &buffer);

            GLuint GetHandle() const {
                return mHandle;
            }

        protected:
            void OnReleased() const override;

            GLuint mHandle = 0;
        };

    }
}

#endif //BERSERK_GLVERTEXBUFFER_HPP