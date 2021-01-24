/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLVertexBuffer::GLVertexBuffer(const Desc& desc) {
            mBufferUsage = desc.bufferUsage;
            mSize = desc.size;

            auto buffer = desc.buffer;

            BERSERK_ASSERT(mSize > 0);

            GLDriver::GetDeferredResourceContext().SubmitInit([buffer, this](){
                this->Initialize(buffer);
            });
        }

        GLVertexBuffer::~GLVertexBuffer() {
            if (mHandle) {
                glDeleteBuffers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
            }

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release vertex buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
        }

        void GLVertexBuffer::Initialize(const Ref<MemoryBuffer>& buffer) {
            auto usage = GLDefs::GetBufferUsage(mBufferUsage);

            BERSERK_ASSERT(buffer.IsNull() || buffer->GetSize() == mSize);

            glGenBuffers(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ARRAY_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferData(GL_ARRAY_BUFFER, mSize, buffer? buffer->GetData(): nullptr, usage);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init vertex buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
        }

        void GLVertexBuffer::Update(uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) {
            BERSERK_ASSERT(byteSize > 0);
            BERSERK_ASSERT(byteOffset + byteSize <= mSize);
            BERSERK_ASSERT(memory.IsNotNull());

            glBindBuffer(GL_ARRAY_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferSubData(GL_ARRAY_BUFFER, byteOffset, byteSize, memory->GetData());
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Update vertex buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
        }

        void GLVertexBuffer::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                Platform::Memory::Release(this);
            });
        }

    }
}