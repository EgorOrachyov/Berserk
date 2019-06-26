//
// Created by Egor Orachyov on 10.06.2019.
//

#ifndef BERSERK_ILOGMANAGER_H
#define BERSERK_ILOGMANAGER_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Logging/ELogVerbosity.h>
#include <IO/Printer.h>
#include <Misc/Exception.h>

namespace Berserk
{

    /**
     * Log manager interface. Log allows to write messages into it
     * from every place in the code in time of execution.
     */
    class CORE_API ILogManager
    {
    public:

        /** Allows to capture stack memory to uses formatted print in buffer*/
        static const uint32 WRITE_BUFFER_SIZE = KiB;

        /** Buffer to format message for custom log output  */
        static const uint32 WRITE_MESSAGE_SIZE = KiB;

    public:

        virtual ~ILogManager() = default;

        /**
         * Adds message into log
         *
         * @param message Buffer with actual content to print
         * @param verbosity Type of message
         * @param mirrorToOutput Set in true to mirror in dev console (if supported)
         */
        virtual void addMessage(const char* message, ELogVerbosity verbosity,
                                bool mirrorToOutput) = 0;

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
         * Allows to add formatted message into log
         * @note Max length of the formatted message = WRITE_BUFFER_SIZE
         *
         * @tparam TArgs Types of the arguments
         * @param verbosity Type of message
         * @param mirrorToOutput Set in true to mirror in dev console (if supported)
         * @param format Formatted string
         * @param args Arguments to insert in the formatted string
         */
        template <typename ... TArgs>
        void addMessagef(ELogVerbosity verbosity, bool mirrorToOutput,
                         const char *format, const TArgs &... args);

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

    template<typename... TArgs>
    void ILogManager::addMessagef(ELogVerbosity verbosity, bool mirrorToOutput, const char *format,
                                  const TArgs &... args)
    {
        char buffer[WRITE_BUFFER_SIZE];
        int32 written = Printer::print(buffer, WRITE_BUFFER_SIZE, format, args ...);

        if (written < 0) throw Exception("ILogManager: cannot write to log");
        else this->addMessage(buffer, verbosity, mirrorToOutput);
    }


} // namespace Berserk

#endif //BERSERK_ILOGMANAGER_H