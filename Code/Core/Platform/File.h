/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_FILE_H
#define BERSERK_FILE_H

#include <String/CString.h>
#include <ErrorList.h>

namespace Berserk {

    enum class EFileFlag : uint32 {
        Read = BERSERK_BIT_SHIFT(0u),
        Write = BERSERK_BIT_SHIFT(1u)
    };
    typedef uint32 EFileFlags;

    /** Platform independent abstraction for file access */
    class File {
    public:
        virtual ~File() = default;
        /** @return True if file opened */
        virtual bool isOpen() const = 0;
        /** @return File flags of the opened file */
        virtual EFileFlags getFlags() const = 0;
        /** @return Path of the file */
        virtual const CString &getFilePath() const = 0;

        /** Close file, no operation with file could not be done after this call */
        virtual void close() = 0;
        /** Flush file content on the disk */
        virtual void flush() = 0;
        /** Set reading position in the file (pos relative to the file beginning) */
        virtual void seek(uint64 position) = 0;

        /**
         * Read file content into buffer
         * @param destination Where to store data
         * @param size Data amount in bytes
         * @return Error if failed to read
         */
        virtual Error read(void* destination, uint64 size) = 0;
        /**
         * Write buffer content into file
         * @param source Data to write into file
         * @param size Data amount in bytes
         * @return Error if failed to write
         */
        virtual Error write(const void* source, int64 size) = 0;
    };

}

#endif //BERSERK_FILE_H