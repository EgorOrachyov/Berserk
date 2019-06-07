//
// Created by Egor Orachyov on 06.06.2019.
//

#ifndef BERSERK_PLATFORMFILE_H
#define BERSERK_PLATFORMFILE_H

#include <IO/IFile.h>
#include <Misc/Include.h>
#include <IO/FileNotFoundException.h>

namespace Berserk
{

    /** Platform C-style implementation of file */
    class PlatformFile : public IFile
    {
    public:

        /**
         * Creates or opens file from full file name string.
         * Acquires OS file handle.
         * @param fullFileName
         * @param writable
         */
        explicit PlatformFile(const char *fullFileName, bool readable, bool writable);

        ~PlatformFile() override;

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

        /** C-style os file handler */
        FILE* mFileHandler = nullptr;

        /** If can read from file */
        bool mReadable = false;

        /** If can write to file */
        bool mWritable = false;

    };

} // namespace Berserk

#endif //BERSERK_PLATFORMFILE_H