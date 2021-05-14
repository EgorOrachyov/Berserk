/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLDriver.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

#include <BerserkCore/Platform/ThreadManager.hpp>

namespace Berserk {
    namespace RHI {

        GLIndexBuffer::GLIndexBuffer(const Desc& desc) {
            mBufferUsage = desc.bufferUsage;
            mSize = desc.size;

            auto buffer = desc.buffer;

            assert(mSize > 0);

            GLDriver::GetDeferredResourceContext().SubmitInit([buffer, this](){
                this->Initialize(buffer);
            });
        }

        GLIndexBuffer::~GLIndexBuffer() {
            if (mHandle) {
                glDeleteBuffers(1, &mHandle);
                BERSERK_GL_CATCH_ERRORS();

                mHandle = 0;
            }

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release index buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLIndexBuffer::Initialize(const Ref<MemoryBuffer>& buffer) {
            auto usage = GLDefs::GetBufferUsage(mBufferUsage);

            assert(buffer.IsNull() || buffer->GetSize() == mSize);

            glGenBuffers(1, &mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mSize, buffer? buffer->GetData(): nullptr, usage);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init index buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLIndexBuffer::Update(uint32 byteOffset, uint32 byteSize, const Ref<MemoryBuffer> &memory) {
            assert(byteSize > 0);
            assert(byteOffset + byteSize <= mSize);
            assert(memory.IsNotNull());

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
            BERSERK_GL_CATCH_ERRORS();

            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, byteOffset, byteSize, memory->GetData());
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            BERSERK_GL_LOG_INFO(BERSERK_TEXT("Update index buffer: thread=\"{0}\""), ThreadManager::GetCurrentThread()->GetName());
        }

        void GLIndexBuffer::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                Memory::Release(this);
            });
        }

    }
}
