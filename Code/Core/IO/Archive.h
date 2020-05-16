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

        void write(uint8 v);
        void write(uint32 v);
        void write(uint64 v);
        void write(int32 v);
        void write(float v);
        void write(bool v);
        void write(const char* string);
        void write(const CString& string);
        void write(const CStringStatic& string);

        void read(uint8& v);
        void read(uint32& v);
        void read(uint64& v);
        void read(int32& v);
        void read(float& v);
        void read(bool& v);
        void read(char* string, uint32 size);
        void read(CString& string);
        void read(CStringStatic& string);

        friend Archive& operator<<(Archive& archive, const uint8& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const uint32& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const uint64& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const int32& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const float& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const bool& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const char* object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const CString& object) { archive.write(object); return archive; }
        friend Archive& operator<<(Archive& archive, const CStringStatic& object) { archive.write(object); return archive; }

        friend Archive& operator>>(Archive& archive, uint8& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, uint32& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, uint64& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, int32& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, float& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, bool& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, CString& object) { archive.read(object); return archive; }
        friend Archive& operator>>(Archive& archive, CStringStatic& object) { archive.read(object); return archive; }

    protected:
        EArchiveType mArchiveType = EArchiveType::Invalid;
        bool mCanRead = false;
        bool mCanWrite = false;
        CString mArchiveName;
        uint64 mArchiveSize = 0;
    };

}

#endif //BERSERK_ARCHIVE_H