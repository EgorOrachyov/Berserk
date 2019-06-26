//
// Created by Egor Orachyov on 26.06.2019.
//

#ifndef BERSERK_IOUTPUTDEVICE_H
#define BERSERK_IOUTPUTDEVICE_H

#include <IO/Printer.h>
#include <Strings/String.h>

namespace Berserk
{
    /**
     * Interface for a device which supports some
     * formatted and simple text output
     */
    class CORE_EXPORT IOutputDevice
    {
    public:

        virtual ~IOutputDevice() = default;

        /**
         * Prints string message into the device
         * @param string C-style string to print
         */
        virtual void print(const char* string) = 0;

        /**
         * Prints string message into the device
         * @param string Dynamic engine string with message
         */
        void print(const String& string)
        {
            print(string.get());
        }

        /**
         * Prints string message into the device
         * @param string Static engine string with message
         * @tparam size Actual buffer size of the static string
         */
        template <uint32 size>
        void print(const StringStatic<char, '\0', size>& string)
        {
            print(string.get());
        }

    };

} // namespace Berserk

#endif //BERSERK_IOUTPUTDEVICE_H