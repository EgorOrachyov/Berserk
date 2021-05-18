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

#ifndef BERSERK_GLVAOCACHE_HPP
#define BERSERK_GLVAOCACHE_HPP

#include <BerserkRHI/RHIVertexBuffer.hpp>
#include <BerserkRHI/RHIIndexBuffer.hpp>
#include <BerserkRHI/RHIVertexDeclaration.hpp>
#include <BerserkCore/Templates/Contracts.hpp>
#include <BerserkCore/Platform/Crc32.hpp>
#include <GL/glew.h>

namespace Berserk {

    namespace RHI {

        class GLVaoCache {
        public:

            struct VaoDescriptor {
                ArrayFixed<RefCounted<RHI::VertexBuffer>> buffers;
                RefCounted<RHI::IndexBuffer> indices;
                RefCounted<RHI::VertexDeclaration> declaration;
            };

            struct VaoKey {
                RefCounted<RHI::VertexBuffer> buffers[RHI::Limits::MAX_VERTEX_BUFFERS];
                RefCounted<RHI::IndexBuffer> indices;
                RefCounted<RHI::VertexDeclaration> declaration;
            };

            struct VaoValue {
                GLuint mHandle;
                uint32 mFrameUsed;
            };

            static const uint32 RELEASE_FREQUENCY = 2;
            static const uint32 TIME_TO_KEEP = 2;

            explicit GLVaoCache(uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
            ~GLVaoCache();

            /** Attempts to look up and find suitable vao. If fails, creates new cache entry */
            GLuint GetOrCreateVao(const VaoDescriptor& descriptor);

            /** Erase cache entries which exceeded time to live */
            void GC();

        private:
            void CreateVaoKey(const VaoDescriptor& descriptor, VaoKey& key);
            void CreateVaoObject(const VaoDescriptor& descriptor, VaoValue& vao);
            void ReleaseVaoObject(VaoValue& vao);

            Map<VaoKey, VaoValue> mEntries;
            uint32 mReleaseFrequency = 2;
            uint32 mTimeToKeep = 4;
            uint32 mLastRelease = 0;
            uint32 mCurrentFrame = 0;
        };

    }

    template<>
    class Equals<RHI::GLVaoCache::VaoKey> {
    public:
        uint32 operator()(const RHI::GLVaoCache::VaoKey& a, const RHI::GLVaoCache::VaoKey& b) const {
            for (size_t i = 0; i < RHI::Limits::MAX_VERTEX_BUFFERS; i++) {
                if (a.buffers[i].Get() != b.buffers[i].Get())
                    return false;
            }

            return a.declaration == b.declaration && a.indices == b.indices;
        }
    };

    template<>
    class Hash<RHI::GLVaoCache::VaoKey> {
    public:
        uint32 operator()(const RHI::GLVaoCache::VaoKey& key) const {
            Crc32Builder builder;

            for (const auto& b: key.buffers)
                builder.Hash(key.buffers, sizeof(RHI::VertexBuffer) * RHI::Limits::MAX_VERTEX_BUFFERS);

            auto indices = key.indices.Get();
            builder.Hash(&indices, sizeof(RHI::IndexBuffer*));

            auto declaration = key.declaration.Get();
            builder.Hash(&declaration, sizeof(RHI::VertexDeclaration*));

            return builder.GetHash();
        }
    };
}

#endif //BERSERK_GLVAOCACHE_HPP