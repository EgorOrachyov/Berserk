//
// Created by Egor Orachyov on 26.06.2019.
//

#ifndef BERSERK_DEFAULTOUTPUTDEVICE_H
#define BERSERK_DEFAULTOUTPUTDEVICE_H

#include <Misc/Include.h>
#include <IO/IOutputDevice.h>

namespace Berserk
{

    /** Default wrapper for stdout put to the console */
    class CORE_EXPORT DefaultOutputDevice : public IOutputDevice
    {
    public:

        /** @copydoc IOutputDevice::print() */
        void print(const char *string) override
        {
            fprintf(stdout, "%s", string);
        }

    };

    /** Static methods for quick access */
    class CORE_EXPORT OutputDevice
    {
    public:

        /** @copydoc IOutputDevice::print() */
        static void print(const char *string)
        { get().print(string); }

        /** @copydoc IOutputDevice::printf() */
        template <typename ... TArgs>
        static void printf(const char* format, const TArgs& ... args)
        { get().printf(format, args ...); }

        /** An instance for passing functions and other objects as param */
        static DefaultOutputDevice& get()
        {
            static DefaultOutputDevice device;
            return device;
        }

    };

} // namespace Berserk

#endif //BERSERK_DEFAULTOUTPUTDEVICE_H