//
// Created by Egor Orachyov on 02.02.2019.
//

#ifndef BERSERK_STRINGPOOL_H
#define BERSERK_STRINGPOOL_H

#include "Public/Memory/PoolAllocator.h"

namespace Berserk
{

    class StringPool
    {
    public:

        enum StringSizes
        {
            Length32    = 0,
            Length64       ,
            Length128      ,
            Length256      ,
            Length512      ,
            Length1024     ,

            Supported
        };

        static const uint32 MIN_STRING_LENGTH = StringSizes::Length32;

        static const uint32 MAX_STRING_LENGTH = StringSizes::Length1024;

    private:

        static const uint32 NODE_INFO_OFFSET = 8;

        struct PoolNode
        {
            uint16 mSize;               // Total buffer size
            uint16 mLength;             // Number of used symbols without \0
            uint32 mReferenceCount;     // Number of references to this string
        };

        friend class DynamicString;

        StringPool();

        ~StringPool() = default;

        void* allocC(StringSizes size);

        void* allocW(StringSizes size);

        void  freeC(StringSizes size, void* pointer);

        void  freeW(StringSizes size, void* pointer);

    public:

        static StringPool& getSingleton();

    private:

        PoolAllocator mCPool[StringSizes::Supported];
        PoolAllocator mWPool[StringSizes::Supported];

    };

} // namespace Berserk

#endif //BERSERK_STRINGPOOL_H
