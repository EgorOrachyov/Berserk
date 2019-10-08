//
// Created by Egor Orachyov on 30.06.2019.
//

#ifndef BERSERK_ARCHIVEFILEWRITER_H
#define BERSERK_ARCHIVEFILEWRITER_H

#include <IO/IFile.h>
#include <Strings/String.h>
#include <Serialization/ArchiveWriter.h>

namespace Berserk
{

    /**
     * Archive for writing to the file
     */
    class CORE_API ArchiveFileWriter : public ArchiveWriter
    {
    public:

        /**
         * Initialize archive writer from file handler and
         * actual path to the file for debug purposes
         * @param file File handler
         * @param filename Name of the file on the disc
         */
        ArchiveFileWriter(IFile &file, const char* filename);

        ~ArchiveFileWriter() override = default;

        /** @copydoc ArchiveFileWriter::serialize() */
        void serialize(const void *source, uint64 bytesToWrite) override;

        /** @copydoc ArchiveFileWriter::getSize() */
        uint32 getSize() const override { mSerializedBytes; }

        /** @copydoc ArchiveFileWriter::isNetworkArchive() */
        bool isNetworkArchive() const override { return false; }

        /** @copydoc ArchiveFileWriter::isFileArchive() */
        bool isFileArchive() const override { return true; }

        /** @return File name of this archive */
        const String& getFilename() const { return mFileName; }

    private:

        /** Write here */
        IFile& mFile;

        /** Name of the file on the disc */
        String mFileName;

        /** Number of serialized bytes */
        uint32 mSerializedBytes = 0;

    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEFILEWRITER_H