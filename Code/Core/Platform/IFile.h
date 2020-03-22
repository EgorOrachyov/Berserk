/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IFILE_H
#define BERSERK_IFILE_H

#include <String/CString.h>
#include <Error.h>

namespace Berserk {

    enum class EFileMode : uint32 {
        Read = 1,
        Write = 2
    };

    /** Platform independent abstraction for file access */
    class IFile {
    public:
        virtual ~IFile() = default;
        /** Close file, no operation with file could not be done after this call */
        virtual void close() = 0;
        /** Flush file content on the disk */
        virtual void flush() = 0;
        /** Set position from the file beginning */
        virtual void seek(uint64 position) = 0;
        /** Set position from the file ending  */
        virtual void seekEnd(uint64 position) = 0;
        /** @return True if file opened */
        virtual bool isOpen() const = 0;
        /** @return True if EOF reached */
        virtual bool isEof() const = 0;
        /** @return File size (in bytes) */
        virtual uint64 getSize() const = 0;
        /** @return Current position in the file (in bytes) */
        virtual uint64 getPosition() const = 0;
        /** @return Open mode of the file */
        virtual EFileMode getMode() const = 0;
        /** @return Path of the file */
        virtual const CString& getFilePath() const = 0;
        /**
         * Read file content into buffer
         * @param destination Where to store data
         * @param size Data amount in bytes
         * @return Error if failed to read
         */
        virtual EError read(void* destination, uint64 size) = 0;
        /**
         * Write buffer content into file
         * @param source Data to write into file
         * @param size Data amount in bytes
         * @return Error if failed to write
         */
        virtual EError write(const void* source, int64 size) = 0;
    };

}

#endif //BERSERK_IFILE_H