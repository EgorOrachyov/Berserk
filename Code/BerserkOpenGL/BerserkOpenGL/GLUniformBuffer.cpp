/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLUniformBuffer.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLUniformBuffer::GLUniformBuffer(const Desc& desc) {
            mBufferUsage = desc.bufferUsage;
            mSize = desc.size;
            auto buffer = desc.buffer;

            BERSERK_ASSERT(mSize > 0);

            GLDriver::GetDeferredResourceContext().SubmitInit([buffer, this](){
                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init uniform buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
                this->Initialize(buffer);
            });
        }

        GLUniformBuffer::~GLUniformBuffer() {
            if (mHandle) {
                glDeleteBuffers(1, &mHandle);
                mHandle = 0;
            }
        }

        void GLUniformBuffer::Initialize(const Ref<MemoryBuffer>& buffer) {
            auto usage = GLDefs::getBufferUsage(mBufferUsage);

            BERSERK_ASSERT(buffer.IsNull() || buffer->GetSize() == mSize);

            glGenBuffers(1, &mHandle);
            glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
            glBufferData(GL_UNIFORM_BUFFER, mSize, buffer? buffer->GetData(): nullptr, usage);
            glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
        }

        void GLUniformBuffer::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release uniform buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
                Platform::Memory::Release(this);
            });
        }

    }
}
