//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_EXCEPTION_H
#define BERSERK_EXCEPTION_H

#include <Strings/StaticString.h>
#include <Misc/UsageDescriptors.h>
#include <exception>

namespace Berserk
{

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
            // todo
            fprintf(stderr, "BERSERK: Exception: %s\n", mMessage.get());
        }

    protected:

        /** Error text */
        CPath mMessage;

    };

} // namespace Berserk

#endif //BERSERK_EXCEPTION_H