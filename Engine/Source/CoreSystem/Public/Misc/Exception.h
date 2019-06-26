//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_EXCEPTION_H
#define BERSERK_EXCEPTION_H

#include <Strings/StringStatic.h>
#include <Misc/Buffers.h>
#include <Misc/Include.h>
#include <Misc/Compilation.h>
#include <Misc/UsageDescriptors.h>
#include <exception>

namespace Berserk
{

    /** Engine default exception with internal storage for message */
    class CORE_API Exception : public std::exception
    {
    public:

        /** Contains only error message */
        explicit Exception(const char* message) : mMessage(message) {}

        /** @return Error message content */
        const char *what() const noexcept override
        {
            return mMessage.get();
        }

        /**
         * Prints this exception content to the default
         * output log with Fatal marking
         */
        virtual void out()
        {
#if DEBUG
            /** Print raw exception message into the error log */
            fprintf(stderr, "BERSERK: Exception: %s\n", mMessage.get());
#endif
        }

    protected:

        /** Exceptions longer than 2048 chars is not useful */
        typedef StringStatic<char, '\0', Buffers::KiB * 2> ErrorMessage;

        /** Error text */
        ErrorMessage mMessage;

    };

} // namespace Berserk

#endif //BERSERK_EXCEPTION_H