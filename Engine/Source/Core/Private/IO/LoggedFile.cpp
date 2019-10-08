//
// Created by Egor Orachyov on 05.06.2019.
//

#include "IO/LoggedFile.h"

namespace Berserk
{

    LoggedFile::LoggedFile(IFile &file, ILogManager &logManager)
            : mFileHandler(file), mLogManager(logManager)
    {

    }

    LoggedFile::~LoggedFile()
    {
        mFileHandler.flush();
    }

    bool LoggedFile::read(void *destination, uint64 bytesToRead)
    {
        bool result = mFileHandler.read(destination, bytesToRead);
        if (result) mLogManager.addMessagef(Display, false, "LoggedFile: read from file [file: %p][dest: %p][bytes: %ul]", &mFileHandler, destination, bytesToRead);
        else mLogManager.addMessagef(Warning, false, "LoggedFile: failed to read from file [file: %p][dest: %p][bytes: %ul]", &mFileHandler, destination, bytesToRead);

        return result;
    }

    bool LoggedFile::write(const void *source, uint64 bytesToWrite)
    {
        bool result = mFileHandler.write(source, bytesToWrite);
        if (result) mLogManager.addMessagef(Display, false, "LoggedFile: write to file [file: %p][source: %p][bytes: %ul]", &mFileHandler, source, bytesToWrite);
        else mLogManager.addMessagef(Warning, false, "LoggedFile: failed to write to file [file: %p][source: %p][bytes: %ul]", &mFileHandler, source, bytesToWrite);

        return result;
    }

    bool LoggedFile::seek(uint64 position)
    {
        return mFileHandler.seek(position);
    }

    bool LoggedFile::flush()
    {
        return mFileHandler.flush();
    }

    bool LoggedFile::valid()
    {
        return mFileHandler.valid();
    }

    int64 LoggedFile::tell()
    {
        return mFileHandler.tell();
    }

    int64 LoggedFile::size()
    {
        return mFileHandler.size();
    }
} // namespace Berserk