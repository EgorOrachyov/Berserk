//
// Created by Egor Orachyov on 05.06.2019.
//

#include "IO/CachedFileWriter.h"

namespace Berserk
{

    CachedFileWriter::CachedFileWriter(IFile &file, uint32 cacheSize, IAllocator &allocator)
        : mAllocator(allocator), mFile(file), mCacheSize(cacheSize)
    {

    }

    CachedFileWriter::~CachedFileWriter()
    {
        if (mCache != nullptr)
        {
            flushCache();
            mAllocator.free(mCache);
            mCache = nullptr;
            mCacheSize = 0;
            mCachePosition = 0;
        }
    }

    bool CachedFileWriter::read(void *destination, uint64 bytesToRead)
    {
        return false;
    }

    bool CachedFileWriter::write(const void *source, uint64 bytesToWrite)
    {
        allocateCache();

        if (bytesToWrite > mCacheSize)
        {
            if (!flushCache()) return false;
            if (!mFile.write(source, bytesToWrite)) return false;
            mCacheStart = mFile.tell();
            mFileSize = Math::max((uint64)mCacheStart + mCachePosition, mFileSize);
        }
        else
        {
            if (mCachePosition + bytesToWrite <= mCacheSize)
            {
                return writeCache(source, bytesToWrite);
            }
            else
            {
                uint64 writeBefore = mCacheSize - mCachePosition;
                if (!writeCache(source, writeBefore)) return false;
                if (!flushCache()) return false;
                uint64 writeAfter = bytesToWrite - writeBefore;
                if (!writeCache(source, writeAfter)) return false;
            }
        }

        return true;
    }

    bool CachedFileWriter::seek(uint64 position)
    {
        if (!flushCache()) return false;
        mCacheStart = position;
        mCachePosition = 0;
        return true;
    }

    bool CachedFileWriter::flush()
    {
        return mFile.flush();
    }

    bool CachedFileWriter::valid()
    {
        return mFile.valid();
    }

    int64 CachedFileWriter::tell()
    {
        return mCacheStart + mCachePosition;
    }

    int64 CachedFileWriter::size()
    {
        return mFileSize;
    }

    void CachedFileWriter::allocateCache()
    {
        if (mCache == nullptr)
        {
            mCache = (uint8*) mAllocator.allocate((uint32)mCacheSize * sizeof(uint8));
        }
    }

    bool CachedFileWriter::flushCache()
    {
        if (mCachePosition > 0)
        {
            if (!mFile.seek((uint64)mCacheStart)) return false;
            if (!mFile.write(mCache, (uint64) mCachePosition)) return false;
            mCacheStart += mCachePosition;
            mCachePosition = 0;

            mFileSize = Math::max((uint64)mCacheStart + mCachePosition, mFileSize);
        }

        return true;
    }

    bool CachedFileWriter::writeCache(const void *source, uint64 bytesToWrite)
    {
        memcpy(&mCache[mCachePosition], source, bytesToWrite);
        mCachePosition += bytesToWrite;
        mFileSize = Math::max((uint64)mCacheStart + mCachePosition, mFileSize);
        return true;
    }

} // namespace Berserk