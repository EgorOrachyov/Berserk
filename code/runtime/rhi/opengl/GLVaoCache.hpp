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

#include <core/Crc32.hpp>
#include <rhi/RHIBuffer.hpp>
#include <rhi/RHIVertexDeclaration.hpp>

#include <algorithm>
#include <array>
#include <unordered_map>
#include <utility>

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLVaoDescriptor
 * @brief Descriptor of vertex array object
 */
struct GLVaoDescriptor {
    std::array<Ref<RHIVertexBuffer>, RHILimits::MAX_VERTEX_BUFFERS> buffers;
    Ref<RHIIndexBuffer> indices;
    Ref<RHIVertexDeclaration> declaration;

    void Reset() {
        buffers.fill(Ref<RHIVertexBuffer>());
        indices.Reset();
        declaration.Reset();
    }
};

/**
 * @class GLVaoKey
 * @brief Key of vertex array object in the cache
 */
struct GLVaoKey {
    std::array<Ref<RHIVertexBuffer>, RHILimits::MAX_VERTEX_BUFFERS> buffers;
    Ref<RHIIndexBuffer> indices;
    Ref<RHIVertexDeclaration> declaration;
    Crc32Hash hash;

    /** @brief Initialize key from descriptor */
    inline void Setup(const GLVaoDescriptor &descriptor) {
        Memory::Set(this, 0x0, sizeof(GLVaoKey));
        std::copy(descriptor.buffers.begin(), descriptor.buffers.end(), buffers.begin());
        indices = descriptor.indices;
        declaration = descriptor.declaration;
        hash = Crc32::Hash(this, sizeof(GLVaoKey));
    }

    /** @brief Fast compare op */
    inline bool operator==(const GLVaoKey &other) const {
        if (hash != other.hash)
            return false;
        return Memory::Compare(this, &other, sizeof(GLVaoKey));
    };
};

/**
 * @}
 */

BRK_NS_END

namespace std {

    template<>
    struct hash<BRK_NS::GLVaoKey> {
    public:
        size_t operator()(const BRK_NS::GLVaoKey &key) const {
            return key.hash;
        }
    };

}// namespace std

BRK_NS_BEGIN

/**
 * @addtogroup opengl
 * @{
 */

/**
 * @class GLVaoCache
 * @brief Cache of opengl vertex array objects
 */
class GLVaoCache {
public:
    static const uint32 RELEASE_FREQUENCY = 4;
    static const uint32 TIME_TO_KEEP = 4;

    BRK_API explicit GLVaoCache(uint32 releaseFrequency = RELEASE_FREQUENCY, uint32 timeToKeep = TIME_TO_KEEP);
    BRK_API ~GLVaoCache();

    /** Attempts to look up and find suitable vao. If fails, creates new cache entry */
    BRK_API GLuint GetOrCreateVao(const GLVaoDescriptor &descriptor);

    /** Erase cache entries which exceeded time to live */
    BRK_API void GC();

private:
    struct GLVaoValue {
        GLuint handle{};
        uint32 frameUsed{};
    };

private:
    void CreateVaoObject(const GLVaoDescriptor &descriptor, GLVaoValue &vao) const;
    void ReleaseVaoObject(const GLVaoValue &vao) const;

    std::unordered_map<GLVaoKey, GLVaoValue> mEntries;
    uint32 mReleaseFrequency;
    uint32 mTimeToKeep;
    uint32 mLastRelease = 0;
    uint32 mCurrentFrame = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLVAOCACHE_HPP
