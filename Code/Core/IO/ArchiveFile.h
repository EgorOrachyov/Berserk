/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ARCHIVEFILE_H
#define BERSERK_ARCHIVEFILE_H

#include <TPtrUnique.h>
#include <IO/Archive.h>
#include <Platform/File.h>

namespace Berserk {

    /**
     * @brief File based archive
     * Behaves as wrapper for IFile, provides write/read operations.
     */
    class ArchiveFile : public Archive {
    public:

        /**
         * Construct binary archive from file
         * @param file Handle to opened system file
         * @param archiveName Optional archive name for debugging
         */
        explicit ArchiveFile(TPtrUnique<File> &file, CString archiveName = "");
        ~ArchiveFile() override;

        EError vwrite(const void *source, uint64 size) override;
        EError vread(void *destination, uint64 size) override;
        bool vempty() const override;

        /** @return Internal file handler */
        const File& getFile() const { return *mFile; }

    private:
        using Archive::mArchiveType;
        using Archive::mCanRead;
        using Archive::mCanWrite;
        using Archive::mArchiveName;
        using Archive::mArchiveSize;

        TPtrUnique<File> mFile;
    };

}

#endif //BERSERK_ARCHIVEFILE_H