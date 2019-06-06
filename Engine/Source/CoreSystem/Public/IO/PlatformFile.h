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

    class PlatformFile : public IFile
    {
    public:

        explicit PlatformFile(const char* fullFileName, bool create = false);

        ~PlatformFile() override;

        bool read(void *destination, int64 bytesToRead) override;

        bool write(const void *source, int64 bytesToWrite) override;

        bool flush() override;

        int64 tell() override;

        int64 size() override;

    protected:

        /** C-style os file handler */
        FILE* mFileHandler = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_PLATFORMFILE_H