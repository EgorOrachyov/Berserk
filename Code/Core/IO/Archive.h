/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ARCHIVE_H
#define BERSERK_ARCHIVE_H

#include <Typedefs.h>
#include <Error.h>
#include <String/CString.h>
#include <String/CStringStatic.h>

namespace Berserk {

    /** Type of the archive. */
    enum class EArchiveType : uint32 {
        Invalid,   /** No serialization allowed */
        Binary     /** Only for binary data in/out. Not human readable */
    };

    /**
     * @brief Serialization archive
     *
     * Serialization container for various types of output/input
     * data. Mostly used for file/text/binary data in/out operations.
     *
     * Serialization operations of different objects are depend on
     * type of the archive. Therefore, there is required additional check
     * of the archive type before serialization.
     */
    class Archive {
    public:

        virtual ~Archive() = default;

        /** @return Writes raw data in archive. Return error as operation status */
        virtual EError vwrite(const void* source, uint64 size) = 0;
        /** @return Reads raw data in archive. Return error as operation status */
        virtual EError vread(void* destination, uint64 size) = 0;
        /** @return True, if reached end of archive and nothing to read any more */
        virtual bool vempty() const= 0;

        /** @return Archive type */
        EArchiveType getType() const { return mArchiveType; }
        /** @return True if archive allows read operations */
        bool canRead() const { return mCanRead; }
        /** @return True if archive allows write operations */
        bool canWrite() const { return mCanWrite; }
        /** @return Archive name (optional, may be empty) */
        const CString& getName() const { return mArchiveName; }
        /** @return Archive size in bytes */
        uint64 getSize() const { return mArchiveSize; }

        void write(uint32 v);
        void write(float32 v);
        void write(const char* string);
        void write(const CString& string);
        void write(const CStringStatic& string);

        void read(uint32& v);
        void read(float32& v);
        void read(char* string, uint32 size);
        void read(CString& string);
        void read(CStringStatic& string);

        template <typename T>
        friend Archive& operator<<(Archive& archive, const T& object) {
            archive.write(object);
            return archive;
        }

        template <typename T>
        friend Archive& operator>>(Archive& archive, T& object) {
            archive.read(object);
            return archive;
        }

    protected:
        EArchiveType mArchiveType = EArchiveType::Invalid;
        bool mCanRead = false;
        bool mCanWrite = false;
        CString mArchiveName;
        uint64 mArchiveSize = 0;
    };

}

#endif //BERSERK_ARCHIVE_H