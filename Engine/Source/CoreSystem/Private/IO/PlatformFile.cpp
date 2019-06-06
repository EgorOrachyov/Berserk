//
// Created by Egor Orachyov on 06.06.2019.
//

#include "IO/PlatformFile.h"
#include <Misc/Assert.h>

namespace Berserk
{

    PlatformFile::PlatformFile(const char *fullFileName, bool create)
    {
        assert(fullFileName);
        if (create) mFileHandler = fopen(fullFileName, "w");
        else mFileHandler = fopen(fullFileName, "rw");
    }

    PlatformFile::~PlatformFile()
    {
        if (mFileHandler)
        {
            fclose(mFileHandler);
            mFileHandler = nullptr;
        }
    }

    bool PlatformFile::read(void *destination, int64 bytesToRead)
    {
        assert(mFileHandler != nullptr);
        int64 count = fread(destination, 1, (uint64) bytesToRead, mFileHandler);
        return (count == bytesToRead);
    }

    bool PlatformFile::write(const void *source, int64 bytesToWrite)
    {
        assert(mFileHandler != nullptr);
        int64 count = fwrite(source, 1, (uint64) bytesToWrite, mFileHandler);
        return (count == bytesToWrite);
    }

    bool PlatformFile::flush()
    {
        assert(mFileHandler != nullptr);
        int64 success = fflush(mFileHandler);
        return (success == 0);
    }

    int64 PlatformFile::tell()
    {
        assert(mFileHandler != nullptr);
        int64 position = ftell(mFileHandler);
        return position;
    }

    int64 PlatformFile::size()
    {
        assert(mFileHandler != nullptr);
        int64 prev = ftell(mFileHandler);
        fseek(mFileHandler, 0L, SEEK_END);
        int64 size = ftell(mFileHandler);
        fseek(mFileHandler, prev, SEEK_SET);
        return size;
    }

} // namespace Berserk