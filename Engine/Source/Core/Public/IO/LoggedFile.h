//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_LOGGEDFILE_H
#define BERSERK_LOGGEDFILE_H

#include <IO/IFile.h>
#include <Logging/ILogManager.h>

namespace Berserk
{

    /** File which supports logging for called operations */
    class LoggedFile : public IFile
    {
    public:

        /**
         * Creates logged file proxy
         * @param file Handler to actually write data
         * @param logManager Log used to push log messages about writes in file
         */
        LoggedFile(IFile& file, ILogManager& logManager);

        ~LoggedFile() override;

        /** @copydoc IFile::read() */
        bool read(void *destination, uint64 bytesToRead) override;

        /** @copydoc IFile::write() */
        bool write(const void *source, uint64 bytesToWrite) override;

        /** @copydoc IFile::seek() */
        bool seek(uint64 position) override;

        /** @copydoc IFile::flush() */
        bool flush() override;

        /** @copydoc IFile::valid() */
        bool valid() override;

        /** @copydoc IFile::tell() */
        int64 tell() override;

        /** @copydoc IFile::size() */
        int64 size() override;

    protected:

        IFile& mFileHandler;
        ILogManager& mLogManager;

    };

} // namespace Berserk




#endif //BERSERK_LOGGEDFILE_H
