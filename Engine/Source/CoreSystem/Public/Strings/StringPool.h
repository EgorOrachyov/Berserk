//
// Created by Egor Orachyov on 02.02.2019.
//

#ifndef BERSERK_STRINGPOOL_H
#define BERSERK_STRINGPOOL_H

#include "Memory/PoolAllocator.h"

namespace Berserk
{

    class StringPool
    {
    public:

        enum StringSizes : uint32
        {
            Length32    = 32,
            Length64    = 64,
            Length128   = 128,
            Length256   = 256,
            Length512   = 512,
            Length1024  = 1024,

            Supported   = 6
        };

        struct PoolNode
        {
            uint16 mSize = 0;               // Total buffer size (in bytes)
            uint16 mLength = 0;             // Number of used symbols without \0
            uint32 mReferenceCount = 0;     // Number of references to this string
        };

        static const uint32 NODE_INFO_OFFSET  = sizeof(PoolNode);
        static const uint32 MIN_BUFFER_SIZE = StringSizes::Length32;
        static const uint32 MAX_BUFFER_SIZE = StringSizes::Length1024;

    public:

        StringPool();

        ~StringPool() = default;

        PoolNode* create();

        PoolNode* allocate(uint32 size);

        void free(PoolNode* node);

    public:

        /** @return Default pool node for engine */
        static StringPool& getSingleton();

        /** @return Pointer to string from pointer to PoolNode */
        static void* getBufferPtr(PoolNode* node);

        /** @return Pointer to PoolNode from pointer to string */
        static void* getNodePtr(void* buffer);

        /** @return Best fit index buffer for specified size */
        static uint32 getBestFit(uint32 size);

        /** @return Buffer index from PoolNode string size */
        static uint32 getBufferIndex(uint32 size);

        /** @return Pool chunk size for nodes with specified string buffers */
        static uint32 getChunkSize(uint32 bufferSize);

    private:



        /** */
        static const uint16 POOL_STRING_SIZES[Supported];

        PoolNode mCreateNode;                           //
        PoolAllocator mPool[StringSizes::Supported];    //

    };

} // namespace Berserk

#endif //BERSERK_STRINGPOOL_H
