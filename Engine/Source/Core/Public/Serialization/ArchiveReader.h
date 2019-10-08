//
// Created by Egor Orachyov on 30.06.2019.
//

#ifndef BERSERK_ARCHIVEREADER_H
#define BERSERK_ARCHIVEREADER_H

#include <HAL/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * An interface for archive class which allows to deserialize game
     * objects and an arbitrary data in uniform way from previously saved
     * archive writer object.
     */
    class CORE_API ArchiveReader
    {
    public:

        virtual ~ArchiveReader() = default;

        /**
         * Raw deserialize read of data
         * @param destination Pointer to the buffer to save data
         * @param bytesToRead Number of bytes to actually read from archive and write in the buffer
         */
        virtual void deserialize(void* destination, uint64 bytesToRead) = 0;

        /**
         * Generic deserialization function for a game object
         * (raw memory read)
         *
         * @tparam T Type of the object to deserialize
         * @param archive Archive used to deserialize data
         * @param arg Object to deserialize
         *
         * @return archive
         */
        template <typename T>
        friend ArchiveReader& operator>>(ArchiveReader& archive, T& arg)
        {
            archive.deserialize(&arg, sizeof(T));
            return archive;
        }

        /** @return Total number of stored data [in bytes] */
        virtual uint32 getSize() const = 0;

        /** @return True if it is network archive */
        virtual bool isNetworkArchive() const { return false; }

        /** @return True if its local file archive */
        virtual bool isFileArchive() const { return false; }

    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEREADER_H