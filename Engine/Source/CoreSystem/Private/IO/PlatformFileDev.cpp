//
// Created by Egor Orachyov on 10.06.2019.
//

#include "IO/PlatformFileDev.h"
#include <Misc/AssertDev.h>
#include <Exception/CoreException.h>

namespace Berserk
{

    PlatformFileDev::PlatformFileDev(const char *fullFileName, bool readable, bool writable)
            : mReadable(readable), mWritable(writable)
    {
        if (fullFileName == nullptr) throw CoreException("PlatformFileDev: null file name");

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

    PlatformFileDev::~PlatformFileDev()
    {
        if (mFileHandler)
        {
            fclose(mFileHandler);
            mFileHandler = nullptr;
        }
    }

    bool PlatformFileDev::read(void *destination, uint64 bytesToRead)
    {
        if (!mReadable) return false;

        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 count = fread(destination, 1, (uint64) bytesToRead, mFileHandler);
        return (count == bytesToRead);
    }

    bool PlatformFileDev::write(const void *source, uint64 bytesToWrite)
    {
        if (!mWritable) return false;

        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 count = fwrite(source, 1, bytesToWrite, mFileHandler);
        return (count == bytesToWrite);
    }

    bool PlatformFileDev::seek(uint64 position)
    {
        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 success = fseek(mFileHandler, position, SEEK_SET);
        return (success == 0);
    }

    bool PlatformFileDev::flush()
    {
        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 success = fflush(mFileHandler);
        return (success == 0);
    }

    bool PlatformFileDev::valid()
    {
        return (mFileHandler != nullptr);
    }

    int64 PlatformFileDev::tell()
    {
        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 position = ftell(mFileHandler);
        return position;
    }

    int64 PlatformFileDev::size()
    {
        if (mFileHandler == nullptr) throw CoreException("PlatformFileDev: null file handler");
        int64 prev = ftell(mFileHandler);
        fseek(mFileHandler, 0L, SEEK_END);
        int64 size = ftell(mFileHandler);
        fseek(mFileHandler, prev, SEEK_SET);
        return size;
    }

} // namespace Berserk