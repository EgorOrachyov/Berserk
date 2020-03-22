/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/ArchiveFile.h>

namespace Berserk {

    ArchiveFile::ArchiveFile(TPtrUnique<IFile> &file, CString archiveName) : Archive() {
        BERSERK_COND_ERROR_FAIL(file.isNotNull() && file->isOpen(), "Archive file must be valid and not null");

        mCanRead = (file->getMode() == EFileMode::Read);
        mCanWrite = (file->getMode() == EFileMode::Write);

        mFile = std::move(file);
        mArchiveType = EArchiveType::Binary;
        mArchiveName = std::move(archiveName);
        mArchiveSize = mFile->getSize();

        BERSERK_COND_ERROR(mCanRead || mCanWrite, "Archive does not provide any read/write operations");
    }

    ArchiveFile::~ArchiveFile() {
        mFile->flush();
        mFile->close();
    }

    EError ArchiveFile::vwrite(const void *source, uint64 size) {
        if (!mCanWrite) return EError::FAILED_WRITE;
        auto status =  mFile->write(source, size);
        if (status == EError::OK) mArchiveSize += size;
        return status;
    }

    EError ArchiveFile::vread(void *destination, uint64 size) {
        if (!mCanRead) return EError::FAILED_READ;
        return mFile->read(destination, size);
    }

    bool ArchiveFile::vempty() const {
        return mFile->isEof();
    }


}