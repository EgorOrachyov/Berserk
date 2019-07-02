//
// Created by Egor Orachyov on 02.07.2019.
//

#ifndef BERSERK_ICONSOLEOBJECT_H
#define BERSERK_ICONSOLEOBJECT_H

#include <Misc/Bits.h>
#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>

namespace Berserk
{

    /**
     * Modification/access types
     */
    enum CORE_API EConsoleObjectFlags
    {
        /** Set in the constructor as default */
        Default     = SHIFT(0),

        /** Cannot be modified */
        ReadOnly    = SHIFT(1),

        /** Could be called from any thread */
        ThreadSafe  = SHIFT(2),

        /** Could not been seen in release mode */
        Cheat       = SHIFT(3)
    };

    /**
     * Modification/call priority order
     */
    enum CORE_API EConsolePriority
    {
        /** Could be set values loaded from config files */
        SetByConfig = 0,

        /** Could be set via raw c++ call */
        SetByCode = 1,

        /** Could be set in console */
        SetByConsole = 2,
    };

    /**
     * Base class for console variable and console command objects
     */
    class CORE_API IConsoleObject
    {
    public:

        virtual ~IConsoleObject() = default;

        /** @return Name of this console object */
        virtual const String& getName() const = 0;

        /** @return Multi-line help text for this object */
        virtual const String& getHelp() const = 0;

        /** @return Object access/behaviour flags */
        virtual EConsoleObjectFlags getFlags() const = 0;

        /** @return Modification/call priority for this object */
        virtual EConsolePriority getPriority() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_ICONSOLEOBJECT_H