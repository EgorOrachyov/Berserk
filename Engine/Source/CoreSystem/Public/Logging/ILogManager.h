//
// Created by Egor Orachyov on 10.06.2019.
//

#ifndef BERSERK_ILOGMANAGER_H
#define BERSERK_ILOGMANAGER_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Logging/ELogVerbosity.h>

namespace Berserk
{

    /**
     * Log manager interface. Log allows to write messages into it
     * from every place in the code in time of execution.
     */
    class CORE_API ILogManager
    {
    public:

        /**
         * Adds message into log
         *
         * @param message Buffer with actual content to print
         * @param verbosity Type of message
         * @param mirrorToOutput Set in true to mirror in dev console (if supported)
         */
        virtual void addMessage(const char* message, ELogVerbosity verbosity, bool mirrorToOutput) = 0;

        /**
         * Adds new message into log and adds additional
         * info about its category
         *
         * @param category Category of the message (or a group)
         * @param message Buffer with actual content to print
         * @param verbosity Type of message
         * @param mirrorToOutput Set in true to mirror in dev console (if supported)
         */
        virtual void addMessage(const char* category, const char* message,
                                ELogVerbosity verbosity, bool mirrorToOutput) = 0;

        /**
         * Appends new page into log
         */
        virtual void addPage() = 0;

        /**
         * Allows to get the verbosity of this log
         * (how actually the log is filtered)
         * @return Verbosity
         */
        virtual ELogVerbosity getVerbosity() const = 0;

    };


} // namespace Berserk

#endif //BERSERK_ILOGMANAGER_H