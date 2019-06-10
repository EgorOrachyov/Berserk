//
// Created by Egor Orachyov on 10.06.2019.
//

#ifndef BERSERK_PLATFORMFILEDEV_H
#define BERSERK_PLATFORMFILEDEV_H

#include <IO/IFile.h>
#include <Misc/Include.h>

namespace Berserk
{

    /**
     * Platform C-style implementation of file for default log
     * manager to support log based assertions in other classes
     */
    class CORE_API PlatformFileDev : public IFile
    {
    public:

        /**
         * Creates or opens file from full file name string.
         * Acquires OS file handle.
         * @param fullFileName
         * @param writable
         */
        explicit PlatformFileDev(const char *fullFileName, bool readable = true, bool writable = false);

        ~PlatformFileDev() override;

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

        /** @return True, if this file handler has not null file */
        bool notNull() { return mFileHandler != nullptr; }

    protected:

        /** C-style os file handler */
        FILE* mFileHandler = nullptr;

        /** If can read from file */
        bool mReadable = false;

        /** If can write to file */
        bool mWritable = false;

    };

} // namespace Berserk

#endif //BERSERK_PLATFORMFILEDEV_H
