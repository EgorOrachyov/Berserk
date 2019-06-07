//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_IFILE_H
#define BERSERK_IFILE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <IO/FileNotFoundException.h>

namespace Berserk
{

    /**
     * File handler interface. Captures unique file low level id. Should
     * be used to get access to non-shared files. Does not support coping.
     */
    class IO_API IFile
    {
    public:

        virtual ~IFile() = default;

        /**
         * Reads specified number of bytes from file
         * @param destination Pointer to buffer to store data
         * @param bytesToRead Count of bytes to be read
         * @return True, if operation completed successfully
         */
        virtual bool read(void *destination, int64 bytesToRead) = 0;

        /**
         * Writes specified bytes to the file
         * @param source Pointer to buffer with data
         * @param bytesToWrite Count of bytes to be written
         * @return True, if operation completed successfully
         */
        virtual bool write(const void *source, int64 bytesToWrite) = 0;

        /**
         * Sets new position of reading or writing from the beginning of the file
         * @param position New postion in file
         * @return True, if operation completed successfully
         */
        virtual bool seek(int64 position) = 0;

        /**
         * Force flush process of the file content to
         * the physical disk (storage)
         * @return True, if operation completed successfully
         */
        virtual bool flush() = 0;

        /**
         * Checks whether this object captures OS file resource
         * @return True, if this file posses OS file resource
         */
        virtual bool valid() = 0;

        /**
         * Return current position of reading / writing
         * @return Byte index position
         */
        virtual int64 tell() = 0;

        /**
         * Total size of the file
         * @return Size in bytes
         */
        virtual int64 size() = 0;

    };

} // namespace Berserk

#endif //BERSERK_IFILE_H