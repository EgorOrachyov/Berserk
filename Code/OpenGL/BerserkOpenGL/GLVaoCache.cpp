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

#include <BerserkOpenGL/GLVaoCache.hpp>
#include <BerserkOpenGL/GLIndexBuffer.hpp>
#include <BerserkOpenGL/GLVertexBuffer.hpp>
#include <BerserkOpenGL/GLDefs.hpp>

namespace Berserk {
    namespace RHI {

        GLVaoCache::GLVaoCache(uint32 releaseFrequency, uint32 timeToKeep) {
            mReleaseFrequency = releaseFrequency;
            mTimeToKeep = timeToKeep;
        }

        GLVaoCache::~GLVaoCache() {
            for (auto& entry: mEntries) {
                auto& vao = entry.GetSecond();
                ReleaseVaoObject(vao);
            }
        }

        GLuint GLVaoCache::GetOrCreateVao(const VaoDescriptor &descriptor) {
            VaoKey key;

            CreateVaoKey(descriptor, key);

            auto value = mEntries.GetPtr(key);

            // Found entry, return it and update frame used info
            if (value != nullptr) {
                value->mFrameUsed = mCurrentFrame;
                return value->mHandle;
            }

            // Create new vao and place it into cache
            VaoValue vao{};

            CreateVaoObject(descriptor, vao);

            GLuint hnd = vao.mHandle;

            // Add entry into the cache
            mEntries.Move(key, vao);

            return hnd;
        }

        void GLVaoCache::GC() {
            if (mCurrentFrame >= mLastRelease + mReleaseFrequency) {
                mLastRelease = mCurrentFrame;

                auto current = mEntries.begin();
                while (current != mEntries.end()) {
                    auto& e = *current;

                    if (e.GetSecond().mFrameUsed + mTimeToKeep <= mCurrentFrame) {
                        ReleaseVaoObject(e.GetSecond());
                        current = mEntries.Remove(current);
                    }
                    else
                        ++current;
                }
            }

            mCurrentFrame += 1;
        }

        void GLVaoCache::CreateVaoKey(const VaoDescriptor &descriptor, VaoKey &key) const {
            // Create valid key for object

            auto& buffers = descriptor.buffers;

            for (auto& buffer: key.buffers)
                buffer = nullptr;
            for (size_t i = 0; i < buffers.GetSize(); i++)
                key.buffers[i] = buffers[i];

            key.indices = descriptor.indices;
            key.declaration = descriptor.declaration;
        }

        void GLVaoCache::CreateVaoObject(const VaoDescriptor &descriptor, VaoValue &vao) const {
            // Create vao value

            auto count = descriptor.buffers.GetSize();
            assert(count > 0);
            assert(descriptor.declaration);

            auto& elements = descriptor.declaration->GetElements();
            auto& buffers = descriptor.buffers;
            auto& indices = descriptor.indices;

            GLuint handle;

            glGenVertexArrays(1, &handle);
            BERSERK_GL_CATCH_ERRORS();

            glBindVertexArray(handle);
            BERSERK_GL_CATCH_ERRORS();

            for (size_t bufferId = 0; bufferId < buffers.GetSize(); bufferId++) {
                auto native = (GLVertexBuffer*) buffers[bufferId].Get();

                glBindBuffer(GL_ARRAY_BUFFER, native->GetHandle());
                BERSERK_GL_CATCH_ERRORS();

                for (size_t location = 0; location < elements.GetSize(); location++) {
                    auto& element = elements[location];

                    GLenum baseType;
                    GLuint components;
                    GLDefs::GetVertexElementType(element.type, baseType, components);

                    if (element.buffer == bufferId) {
                        glEnableVertexAttribArray(location);
                        BERSERK_GL_CATCH_ERRORS();

                        glVertexAttribDivisor(location, element.frequency == VertexFrequency::PerInstance ? 1 : 0);
                        BERSERK_GL_CATCH_ERRORS();

                        const uint8* offset = nullptr;
                        offset = offset + element.offset;

                        glVertexAttribPointer(location, components, baseType, GL_FALSE, element.stride, offset);
                        BERSERK_GL_CATCH_ERRORS();
                    }
                }
            }

            if (indices) {
                auto native = (GLIndexBuffer*) indices.Get();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, native->GetHandle());
                BERSERK_GL_CATCH_ERRORS();
            }

            glBindVertexArray(GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
            BERSERK_GL_CATCH_ERRORS();

            vao.mHandle = handle;
            vao.mFrameUsed = mCurrentFrame;
        }

        void GLVaoCache::ReleaseVaoObject(VaoValue &vao) const {
            glDeleteVertexArrays(1, &vao.mHandle);
            BERSERK_GL_CATCH_ERRORS();
        }

    }
}