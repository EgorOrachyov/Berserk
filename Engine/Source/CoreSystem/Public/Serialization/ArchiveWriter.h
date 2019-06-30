//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_ARCHIVEWRITER_H
#define BERSERK_ARCHIVEWRITER_H

#include <Misc/Types.h>
#include <Misc/Include.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * An interface for archive class which allows to serialize game
     * objects and an arbitrary data in uniform way for saving, loading,
     * network replications and debug purposes.
     */
    class CORE_API ArchiveWriter
    {
    public:

        virtual ~ArchiveWriter() = default;

        /**
         * Raw serialize write of data
         * @param source Pointer to the buffer with data
         * @param bytesToWrite Number of bytes to actually write
         */
        virtual void serialize(const void* source, uint64 bytesToWrite) = 0;

        /**
         * Generic serialize function for a game object
         * (raw memory save)
         *
         * @tparam T Type of the object to serialize
         * @param archive Archive used to serialize data
         * @param arg Object to serialize
         *
         * @return archive
         */
        template <typename T>
        friend ArchiveWriter& operator<<(ArchiveWriter& archive, const T& arg)
        {
            archive.serialize(&arg, sizeof(T));
            return archive;
        }

        /** @return Total number of written data [in bytes] */
        virtual uint32 getSize() const = 0;

        /** @return True if it is network archive */
        virtual bool isNetworkArchive() const { return false; }

        /** @return True if its local file archive */
        virtual bool isFileArchive() const { return false; }
    };

} // namespace Berserk

#endif //BERSERK_ARCHIVEWRITER_H