//
// Created by Egor Orachyov on 06.06.2019.
//

#include "IO/PlatformFile.h"
#include <Misc/Assert.h>

namespace Berserk
{

    PlatformFile::PlatformFile(const char *fullFileName, bool readable, bool writable)
            : mReadable(readable), mWritable(writable)
    {
        assertion(fullFileName);
        if (writable)
        {
            if (readable) mFileHandler = fopen(fullFileName, "rw");
            else mFileHandler = fopen(fullFileName, "w");
        }
        else
        {
            if (readable) mFileHandler = fopen(fullFileName, "r");
            else mFileHandler = nullptr;
        }
    }

    PlatformFile::~PlatformFile()
    {
        if (mFileHandler)
        {
            fclose(mFileHandler);
            mFileHandler = nullptr;
        }
    }

    bool PlatformFile::read(void *destination, uint64 bytesToRead)
    {
        if (!mReadable) return false;

        assertion(mFileHandler != nullptr);
        int64 count = fread(destination, 1, (uint64) bytesToRead, mFileHandler);
        return (count == bytesToRead);
    }

    bool PlatformFile::write(const void *source, uint64 bytesToWrite)
    {
        if (!mWritable) return false;

        assertion(mFileHandler != nullptr);
        int64 count = fwrite(source, 1, bytesToWrite, mFileHandler);
        return (count == bytesToWrite);
    }

    bool PlatformFile::seek(uint64 position)
    {
        assertion(mFileHandler != nullptr);
        int64 success = fseek(mFileHandler, position, SEEK_SET);
        return (success == 0);
    }

    bool PlatformFile::flush()
    {
        assertion(mFileHandler != nullptr);
        int64 success = fflush(mFileHandler);
        return (success == 0);
    }

    bool PlatformFile::valid()
    {
        return (mFileHandler != nullptr);
    }

    int64 PlatformFile::tell()
    {
        assertion(mFileHandler != nullptr);
        int64 position = ftell(mFileHandler);
        return position;
    }

    int64 PlatformFile::size()
    {
        assertion(mFileHandler != nullptr);
        int64 prev = ftell(mFileHandler);
        fseek(mFileHandler, 0L, SEEK_END);
        int64 size = ftell(mFileHandler);
        fseek(mFileHandler, prev, SEEK_SET);
        return size;
    }

} // namespace Berserk