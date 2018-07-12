//
// Created by Egor Orachyov on 10.07.2018.
//

#include "Strings/CStringBuffer.h"
#include "Essential/Assert.h"

namespace Berserk
{
    CStringBuffer globalCStringBuffer;

    CStringBuffer::CStringBuffer()
    {
        mPool[0].init(sizeof(CHAR) * SC_MAX_CAPACITY_32,  32);
        mPool[1].init(sizeof(CHAR) * SC_MAX_CAPACITY_64,  16);
        mPool[2].init(sizeof(CHAR) * SC_MAX_CAPACITY_128,  8);
        mPool[3].init(sizeof(CHAR) * SC_MAX_CAPACITY_256,  4);
        mPool[4].init(sizeof(CHAR) * SC_MAX_CAPACITY_512,  2);
        mPool[5].init(sizeof(CHAR) * SC_MAX_CAPACITY_1024, 1);

        mStringsCount = 0;
        mIsInitialized = true;
    }

    CStringBuffer::~CStringBuffer()
    {
        PUSH("Delete CString buffer %p\n", this);
        destroy();
    }

    void CStringBuffer::init()
    {
        if (isInitialized())
        {
            WARNING("CString buffer is initialized");
            return;
        }

        mPool[0].init(sizeof(CHAR) * SC_MAX_CAPACITY_32,  32);
        mPool[1].init(sizeof(CHAR) * SC_MAX_CAPACITY_64,  16);
        mPool[2].init(sizeof(CHAR) * SC_MAX_CAPACITY_128,  8);
        mPool[3].init(sizeof(CHAR) * SC_MAX_CAPACITY_256,  4);
        mPool[4].init(sizeof(CHAR) * SC_MAX_CAPACITY_512,  2);
        mPool[5].init(sizeof(CHAR) * SC_MAX_CAPACITY_1024, 1);

        mStringsCount = 0;
        mIsInitialized = true;

        for (UINT32 i = 0; i < SBC_CSTRING_BUFFERS_COUNT; i++)
        {
            PUSH("Init pool:  Free blocks:%u  Element size:%u bytes  One buffer capacity: %u \n",
                 mPool[i].getNumOfFreeBlocks(), mPool[i].getElementSize(), mPool[i].getOneBufferCapacity());
        }
    }

    void CStringBuffer::destroy()
    {
        if (isInitialized())
        {
            for (UINT32 i = 0; i < SBC_CSTRING_BUFFERS_COUNT; i++)
            {
                PUSH("Destroy pool:  Free blocks:%u  Element size:%u bytes  One buffer capacity: %u \n",
                     mPool[i].getNumOfFreeBlocks(), mPool[i].getElementSize(), mPool[i].getOneBufferCapacity());

                mPool[i].reset();
            }
        }

        mStringsCount = 0;
        mIsInitialized = false;
    }

    UINT32 CStringBuffer::getStringsCount() const
    {
        return mStringsCount;
    }

    bool CStringBuffer::isInitialized() const
    {
        return mIsInitialized;
    }

    void CStringBuffer::returnBlock(UINT32 size, CHAR* block)
    {
        ASSERT(isInitialized(), "CString buffer is not initialized");

        UINT32 poolId;

        switch (size)
        {
            case SC_MAX_CAPACITY_32: poolId = 0;
                break;

            case SC_MAX_CAPACITY_64: poolId = 1;
                break;

            case SC_MAX_CAPACITY_128: poolId = 2;
                break;

            case SC_MAX_CAPACITY_256: poolId = 3;
                break;

            case SC_MAX_CAPACITY_512: poolId = 4;
                break;

            case SC_MAX_CAPACITY_1024: poolId = 5;
                break;

            default:
                ERROR("Invalid string size");
                ERROR("String buffer cannot return block");

                return;
        }

        mStringsCount -= 1;
        mPool[poolId].freeBlock(block);
    }

    CHAR* CStringBuffer::getBlock(UINT32 size)
    {
        ASSERT(isInitialized(), "CString buffer is not initialized");

        UINT32 poolId;

        switch (size)
        {
            case SC_MAX_CAPACITY_32: poolId = 0;
                break;

            case SC_MAX_CAPACITY_64: poolId = 1;
                break;

            case SC_MAX_CAPACITY_128: poolId = 2;
                break;

            case SC_MAX_CAPACITY_256: poolId = 3;
                break;

            case SC_MAX_CAPACITY_512: poolId = 4;
                break;

            case SC_MAX_CAPACITY_1024: poolId = 5;
                break;

            default:
                ERROR("Invalid string size");
                ERROR("String buffer cannot alloc block");

                return NULL;
        }

        mStringsCount += 1;
        return (CHAR*)mPool[poolId].allocBlock();
    }

    CStringBuffer& CStringBuffer::get()
    {
        return globalCStringBuffer;
    }

} // namespace Berserk