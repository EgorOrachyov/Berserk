//
// Created by Egor Orachyov on 10.07.2018.
//

#include "Strings/WStringBuffer.h"
#include "Essential/Assert.h"

namespace Berserk
{
    WStringBuffer globalWStringBuffer;

    WStringBuffer::WStringBuffer()
    {
        mPool[0].init(sizeof(WCHAR) * SC_MAX_CAPACITY_32,  32);
        mPool[1].init(sizeof(WCHAR) * SC_MAX_CAPACITY_64,  16);
        mPool[2].init(sizeof(WCHAR) * SC_MAX_CAPACITY_128,  8);
        mPool[3].init(sizeof(WCHAR) * SC_MAX_CAPACITY_256,  4);
        mPool[4].init(sizeof(WCHAR) * SC_MAX_CAPACITY_512,  2);
        mPool[5].init(sizeof(WCHAR) * SC_MAX_CAPACITY_1024, 1);

        mStringsCount = 0;
        mIsInitialized = true;

        for (UINT32 i = 0; i < SBC_WSTRING_BUFFERS_COUNT; i++)
        {
            PUSH("Init pool:  Free blocks:%u  Element size:%u bytes  One buffer capacity: %u \n",
                 mPool[i].getNumOfFreeBlocks(), mPool[i].getElementSize(), mPool[i].getOneBufferCapacity());
        }
    }

    WStringBuffer::~WStringBuffer()
    {
        PUSH("Delete WString buffer %p\n", this);
        destroy();
    }

    void WStringBuffer::init()
    {
        if (isInitialized())
        {
            WARNING("WString buffer is initialized");
            return;
        }

        mPool[0].init(sizeof(WCHAR) * SC_MAX_CAPACITY_32,  32);
        mPool[1].init(sizeof(WCHAR) * SC_MAX_CAPACITY_64,  16);
        mPool[2].init(sizeof(WCHAR) * SC_MAX_CAPACITY_128,  8);
        mPool[3].init(sizeof(WCHAR) * SC_MAX_CAPACITY_256,  4);
        mPool[4].init(sizeof(WCHAR) * SC_MAX_CAPACITY_512,  2);
        mPool[5].init(sizeof(WCHAR) * SC_MAX_CAPACITY_1024, 1);

        mStringsCount = 0;
        mIsInitialized = true;

        for (UINT32 i = 0; i < SBC_WSTRING_BUFFERS_COUNT; i++)
        {
            PUSH("Init pool:  Free blocks:%u  Element size:%u bytes  One buffer capacity: %u \n",
                 mPool[i].getNumOfFreeBlocks(), mPool[i].getElementSize(), mPool[i].getOneBufferCapacity());
        }
    }

    void WStringBuffer::destroy()
    {
        if (isInitialized())
        {
            for (UINT32 i = 0; i < SBC_WSTRING_BUFFERS_COUNT; i++)
            {
                PUSH("Destroy pool:  Free blocks:%u  Element size:%u bytes  One buffer capacity: %u \n",
                     mPool[i].getNumOfFreeBlocks(), mPool[i].getElementSize(), mPool[i].getOneBufferCapacity());

                mPool[i].reset();
            }
        }

        mStringsCount = 0;
        mIsInitialized = false;
    }

    UINT32 WStringBuffer::getStringsCount() const
    {
        return mStringsCount;
    }

    bool WStringBuffer::isInitialized() const
    {
        return mIsInitialized;
    }

    void WStringBuffer::returnBlock(UINT32 size, WCHAR* block)
    {
        ASSERT(isInitialized(), "WString buffer is not initialized");

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

    WCHAR* WStringBuffer::getBlock(UINT32 size)
    {
        ASSERT(isInitialized(), "WString buffer is not initialized");

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
        return (WCHAR*)mPool[poolId].allocBlock();
    }

    WStringBuffer& WStringBuffer::get()
    {
        return globalWStringBuffer;
    }

} // namespace Berserk