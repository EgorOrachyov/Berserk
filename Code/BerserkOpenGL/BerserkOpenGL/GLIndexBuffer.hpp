/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_GLINDEXBUFFER_HPP
#define BERSERK_GLINDEXBUFFER_HPP

#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <GL/glew.h>

namespace Berserk {
    namespace RHI {

        class GLIndexBuffer : public IndexBuffer {
        public:
            explicit GLIndexBuffer(const Desc &desc);
            ~GLIndexBuffer() override;

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

#endif //BERSERK_GLINDEXBUFFER_HPP
