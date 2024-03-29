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
                ArrayFixed<RcPtr<RHI::VertexBuffer>> buffers;
                RcPtr<RHI::IndexBuffer> indices;
                RcPtr<RHI::VertexDeclaration> declaration;
            };

            struct VaoKey {
                RcPtr<RHI::VertexBuffer> buffers[RHI::Limits::MAX_VERTEX_BUFFERS];
                RcPtr<RHI::IndexBuffer> indices;
                RcPtr<RHI::VertexDeclaration> declaration;
                CrcHash hash;
            };

            struct VaoValue {
                GLuint handle;
                uint32 frameUsed;
            };

            static const uint32 RELEASE_FREQUENCY = 2;
            static const uint32 TIME_TO_KEEP = 4;

            explicit GLVaoCache(uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
            ~GLVaoCache();

            /** Attempts to look up and find suitable vao. If fails, creates new cache entry */
            GLuint GetOrCreateVao(const VaoDescriptor& descriptor);

            /** Erase cache entries which exceeded time to live */
            void GC();

        private:
            void CreateVaoKey(const VaoDescriptor& descriptor, VaoKey& key) const;
            void CreateVaoObject(const VaoDescriptor& descriptor, VaoValue& vao) const;
            void ReleaseVaoObject(VaoValue& vao) const;

            HashTable<VaoKey, VaoValue> mEntries;
            uint32 mReleaseFrequency;
            uint32 mTimeToKeep;
            uint32 mLastRelease = 0;
            uint32 mCurrentFrame = 0;
        };

    }

}

#endif //BERSERK_GLVAOCACHE_HPP