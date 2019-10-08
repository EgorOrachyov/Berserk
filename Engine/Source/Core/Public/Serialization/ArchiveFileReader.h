//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_ARCHIVEFILEREADER_H
#define BERSERK_ARCHIVEFILEREADER_H

#include <IO/IFile.h>
#include <Strings/String.h>
#include <Serialization/ArchiveReader.h>

namespace Berserk
{

    /**
     * Archive for reading from the file
     */
    class CORE_API ArchiveFileReader : public ArchiveReader
    {
    public:

        /**
         * Initialize archive writer from file handler and
         * actual path to the file for debug purposes
         * @param file File handler
         * @param filename Name of the file on the disc
         */
        ArchiveFileReader(IFile& file, const char* filename);

        ~ArchiveFileReader() override = default;

        /** @copydoc ArchiveFileReader::deserialize() */
        void deserialize(void *destination, uint64 bytesToRead) override;

        /** @copydoc ArchiveFileReader::getSize() */
        uint32 getSize() const override { return (uint32) mFile.size(); }

        /** @copydoc ArchiveFileReader::isNetworkArchive() */
        bool isNetworkArchive() const override { return false; };

        /** @copydoc ArchiveFileReader::isFileArchive() */
        bool isFileArchive() const override { return true; };

        /** @return File name of this archive */
        const String& getFilename() const { return mFileName; }

    private:

        /** Read from here */
        IFile& mFile;

        /** Name of the file on the disc */
        String mFileName;

    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEFILEREADER_H