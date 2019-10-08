//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_ERROR_H
#define BERSERK_ERROR_H

#include <HAL/Platform.h>
#include <HAL/Memory.h>
#include <exception>

namespace Berserk {

    /** Basic throwable object for any type of runtime error */
    class Error : public std::exception {
    public:

        /** Error exception form string message (size is limited) */
        Error(const char* message);

        /** @return Error message (readonly) */
        const char* getMessage() const {
            return (char*) mMessageBuffer;
        }

    private:

        static const uint32 BUFFER_SIZE = Memory::KiB;
        uint8 mMessageBuffer[BUFFER_SIZE] = { '\0' };

    };

}

#endif //BERSERK_ERROR_H