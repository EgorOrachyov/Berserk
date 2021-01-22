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
            mIndexType = desc.indexType;
            mElementsCount = desc.elementsCount;

            auto buffer = desc.buffer;

            BERSERK_ASSERT(mElementsCount > 0);

            GLDriver::GetDeferredResourceContext().SubmitInit([buffer, this](){
                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Init index buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
                this->Initialize(buffer);
            });
        }

        GLIndexBuffer::~GLIndexBuffer() {
            if (mHandle) {
                glDeleteBuffers(1, &mHandle);
                mHandle = 0;
            }
        }

        void GLIndexBuffer::Initialize(const Ref<MemoryBuffer>& buffer) {
            auto usage = GLDefs::getBufferUsage(mBufferUsage);
            auto indexSize = GLDefs::getIndexSize(mIndexType);
            auto size = indexSize * mElementsCount;

            BERSERK_ASSERT(buffer.IsNull() || buffer->GetSize() == size);

            glGenBuffers(1, &mHandle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHandle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer? buffer->GetData(): nullptr, usage);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
        }

        void GLIndexBuffer::OnReleased() const {
            GLDriver::GetDeferredResourceContext().SubmitRelease([this](){
                BERSERK_GL_LOG_INFO(BERSERK_TEXT("Release index buffer: thread=\"{0}\""), Platform::ThreadManager::GetCurrentThread()->GetName());
                Platform::Memory::Release(this);
            });
        }

    }
}
