/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_FILE_HPP
#define BERSERK_FILE_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Strings/String.hpp>

namespace Berserk {
    namespace Platform {

        class File {
        public:

            enum class Mode {
                Read,
                Write
            };

            virtual ~File() = default;

            /** Close file, no operation with file could not be done after this call */
            virtual void Close() = 0;

            /** Flush file content on the disk */
            virtual void Flush() = 0;

            /** Set position from the file beginning */
            virtual void Seek(uint64 position) = 0;

            /** @return True if file opened */
            virtual bool IsOpen() const = 0;

            /** @return True if EOF reached */
            virtual bool IsEof() const = 0;

            /** @return File size (in bytes) */
            virtual size_t GetSize() const = 0;

            /** @return Current position in the file (in bytes) */
            virtual size_t GetPosition() const = 0;

            /** @return Open mode of the file */
            virtual Mode GetAccessMode() const = 0;

            /** @return Path of the file */
            virtual const String& GetFilename() const = 0;

            /**
             * Read file content into buffer
             * @param destination Where to store data
             * @param size Data amount in bytes
             *
             * @return Bytes read
             */
            virtual size_t ReadBytes(void* destination, size_t size) = 0;

            /**
             * Write buffer content into file
             * @param source Data to write into file
             * @param size Data amount in bytes
             *
             * @return Bytes written
             */
            virtual size_t WriteBytes(const void* source, size_t size) = 0;
        };

    }
}

#endif //BERSERK_FILE_HPP