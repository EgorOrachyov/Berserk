//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_ICONSOLEVARIABLE_H
#define BERSERK_ICONSOLEVARIABLE_H

#include <Console/IConsoleObject.h>

namespace Berserk
{

    /**
     * Console variable, that can be set/get via console manager.
     */
    class CORE_API IConsoleVariable
    {
    public:

        /**
         * Set the value of the variable from string
         * @param value String representation of the value (raw value)
         * @param priority Where/how access this value
         * @return True if successfully set new value
         */
        virtual bool set(const char* value, EConsolePriority priority) = 0;

        /** @return Variable data as int value */
        virtual int32 getInt() const = 0;

        /** @return Variable data as float value */
        virtual float32 getFloat() const = 0;

        /** @return Variable data as string value */
        virtual const String& getString() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_ICONSOLEVARIABLE_H