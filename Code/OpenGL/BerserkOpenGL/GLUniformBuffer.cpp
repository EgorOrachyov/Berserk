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

            assert(mSize > 0);

            GLDriver::GetDeferredResourceContext().SubmitInit([buffer, this](){
                this->Initialize(buffer);
            });
        }

        GLUniformBuffer::~GLUniformBuffer() {
            if (mHandle) {
                glDeleteBuffers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
            }

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release uniform buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLUniformBuffer::Initialize(const RefCounted<ReadOnlyMemoryBuffer>& buffer) {
            auto usage = GLDefs::GetBufferUsage(mBufferUsage);

            assert(buffer.IsNull() || buffer->GetSize() == mSize);

            glGenBuffers(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferData(GL_UNIFORM_BUFFER, mSize, buffer? buffer->GetData(): nullptr, usage);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init uniform buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLUniformBuffer::Update(uint32 byteOffset, uint32 byteSize, const RefCounted<ReadOnlyMemoryBuffer> &memory) {
            assert(byteSize > 0);
            assert(byteOffset + byteSize <= mSize);
            assert(memory.IsNotNull());

            glBindBuffer(GL_UNIFORM_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, byteSize, memory->GetData());
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Update uniform buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLUniformBuffer::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                Memory::Release(this);
            });
        }

    }
}
