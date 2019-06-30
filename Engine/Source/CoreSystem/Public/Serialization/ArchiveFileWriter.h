//
// Created by Egor Orachyov on 30.06.2019.
//

#ifndef BERSERK_ARCHIVEFILEWRITER_H
#define BERSERK_ARCHIVEFILEWRITER_H

#include <Serialization/ArchiveWriter.h>
#include <IO/IFile.h>
#include <Resource/TUniquePtr.h>
#include <Strings/String.h>

namespace Berserk
{

    /**
     *
     */
    class CORE_API ArchiveFileWriter : public ArchiveWriter
    {
    public:

        /**
         * Initialize archive writer from file handler and
         * actual path to the file for debug purposes
         * @param file File handler pointer
         * @param filename Name of the file on the disc
         */
        ArchiveFileWriter(const TUniquePtr<IFile> &file, const char* filename);

        ~ArchiveFileWriter() override = default;

        void serialize(const void *source, uint64 bytesToWrite) override;

        uint32 getSize() const override { mSerializedBytes; }

        bool isNetworkArchive() const override { return false; }

        bool isFileArchive() const override { return true; }

    public:

        /** Write here */
        TUniquePtr<IFile> mFile;

        /** Name of the file on the disc */
        String mFileName;

        /** Number of serialized bytes */
        uint32 mSerializedBytes = 0;

    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEFILEWRITER_H