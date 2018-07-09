//
// Created by Egor Orachyov on 24.05.2018.
//

#ifndef BERSERKENGINE_LOGMANAGER_H
#define BERSERKENGINE_LOGMANAGER_H

#include "../Essential/Common.h"
#include "../Essential/Types.h"
#include "../Essential/StandardTimer.h"

#include "Strings/CHARStaticString.h"
#include "LogProperties.h"

namespace Berserk
{
    /**
     *  Singleton log manager for pushing messages into desired or standard log file and
     *  copy that into the console (if running in DEBUG mode)
     */
    class LogManager
    {
    public:

        LogManager();
        ~LogManager();

        /**
         * Open or create new log file to write into it
         *   
         * @param fileName Name of log file (it will be created and written)
         */
        void setLoggingFile(const char *fileName);

        /**
         * Open or create new log file to write into it
         *
         * @param fileName String with filename
         */
        void setLoggingFile(CHARStaticString &fileName);

        /**
         * Push initial message with time, version and common info
         */
        void pushInitialMessage();

        /**
         * Push finale message with common info
         */
        void pushFinalMessage();

        /**
         * Push long '---...---' kine into log
         */
        void pushLine();

        /**
         * Push current time of execution (in seconds)
         */
        void pushCurrentTime();

        /**
         * Starts block of info into the log
         */
        void beginBlock();

        /**
         * Starts block of info into log
         *
         * @param blockName Name of block to mark that
         */
        void beginBlock(const CHAR *blockName);

        /**
         * Starts block of info into the log
         *
         * @param blockName Name of block to mark it
         */
        void beginBlock(CHARStaticString &blockName);

        /**
         * Ends block of info
         */
        void endBlock();

        /**
         * Push free line (equals to new line)
         */
        void pushFreeLine();

        /**
         * Push message into the log
         *
         * @param type Message type
         * @param MSG Text to be pushed
         */
        void pushMessage(LogMessageType type, const CHAR *MSG);

        /**
         * Push message into the log
         *
         * @param type Message type
         * @param MSG String to be pushed
         */
        void pushMessage(LogMessageType type, CHARStaticString &MSG);

        /**
         * Push message into the info block in the log
         *
         * @param MSG Text to be pushed
         */
        void pushMessageBlock(const CHAR *MSG);

        /**
         * Push message into the info block in the log
         *
         * @param MSG String to be pushed
         */
        void pushMessageBlock(CHARStaticString &MSG);

        /**
         * Get number of the next line which could be pushed in the log
         *
         * @return Line Counter
         */
        INT64 getCurrentLineNumber();

        /**
         * Get time from the beginning of Log Manager working
         *
         * @return Time in seconds
         */
        FLOAT64 getCurrentTime();

        /**
         * Get global singleton manager for logging
         *
         * @return Global Log Manager
         */
        static LogManager& getGlobalLogManager();

    private:

        /**
         * Close correctly log file
         */
        void closeLogFile();

        /**
         * Pushes error message into the console with red color of text
         *
         * @param MSG error for console
         */
        void consoleWriteError(const CHAR *MSG);

        /**
         * Pushes info message into the console with standard white color of text
         *
         * @param MSG info for console
         */
        void consoleWriteInfo(const CHAR *MSG);

    private:

        FILE* mLogFile;                 // Current opened file for logging
        StandardTimer mTimer;           // Internal timer
        INT64 mLinesCounter;            // Counter of marked lines (for fast navigation in the log)

    };

    /**
     * Global Singleton Log Manager
     */
    extern LogManager globalLogManager;

} // namespace Berserk

#endif //BERSERKENGINE_LOGMANAGER_H
