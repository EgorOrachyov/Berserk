//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_COREEXCEPTION_H
#define BERSERK_COREEXCEPTION_H

#include <Exception/Exception.h>

namespace Berserk
{

    class CORE_API CoreException : public Exception
    {
    public:

        CoreException(const char *message) : Exception(message) {}

        /** @copydoc Exception::out() */
        void out() override
        {
            // todo
            fprintf(stderr, "BERSERK: CoreSystem: %s\n", mMessage.get());
        }

    };

} // namespace Berserk

#endif //BERSERK_COREEXCEPTION_H