//
// Created by Egor Orachyov on 24.05.2018.
//

#ifndef BERSERKENGINE_LOGMANAGER_H
#define BERSERKENGINE_LOGMANAGER_H

#include "../Essential/Common.h"
#include "../Essential/Types.h"
#include "../Essential/StandardTimer.h"

#include "../Strings/StaticStringASCII.h"
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
        void SetLoggingFile(const char * fileName);

        /**
         * Open or create new log file to write into it
         *
         * @param fileName String with filename
         */
        void SetLoggingFile(StaticStringASCII& fileName);

        /**
         * Push initial message with time, version and common info
         */
        void PushInitialMessage();

        /**
         * Push finale message with common info
         */
        void PushFinalMessage();

        /**
         * Push long '---...---' kine into log
         */
        void PushLine();

        /**
         * Push current time of execution (in seconds)
         */
        void PushCurrentTime();

        /**
         * Starts block of info into the log
         */
        void BeginBlock();

        /**
         * Starts block of info into log
         *
         * @param blockName Name of block to mark that
         */
        void BeginBlock(const CHAR* blockName);

        /**
         * Starts block of info into the log
         *
         * @param blockName Name of block to mark it
         */
        void BeginBlock(StaticStringASCII& blockName);

        /**
         * Ends block of info
         */
        void EndBlock();

        /**
         * Push free line (equals to new line)
         */
        void PushFreeLine();

        /**
         * Push message into the log
         *
         * @param type Message type
         * @param MSG Text to be pushed
         */
        void PushMessage(LogMessageType type, const CHAR* MSG);

        /**
         * Push message into the log
         *
         * @param type Message type
         * @param MSG String to be pushed
         */
        void PushMessage(LogMessageType type, StaticStringASCII& MSG);

        /**
         * Push message into the info block in the log
         *
         * @param MSG Text to be pushed
         */
        void PushMessageBlock(const CHAR* MSG);

        /**
         * Push message into the info block in the log
         *
         * @param MSG String to be pushed
         */
        void PushMessageBlock(StaticStringASCII& MSG);

        /**
         * Get number of the next line which could be pushed in the log
         *
         * @return Line Counter
         */
        int64 GetCurrentLineNumber();

        /**
         * Get time from the beginning of Log Manager working
         *
         * @return Time in seconds
         */
        float64 GetCurrentTime();

        /**
         * Get global singleton manager for logging
         *
         * @return Global Log Manager
         */
        static LogManager& GetGlobalLogManager();

    private:

        /**
         * Close correctly log file
         */
        void CloseLogFile();

        /**
         * Pushes error message into the console with red color of text
         *
         * @param MSG error for console
         */
        void ConsoleWriteError(const CHAR* MSG);

        /**
         * Pushes info message into the console with standard white color of text
         *
         * @param MSG info for console
         */
        void ConsoleWriteInfo(const CHAR* MSG);

    private:

        FILE* mLogFile;                 // Current opened file for logging
        StandardTimer mTimer;           // Internal timer
        int64 mLinesCounter;            // Counter of marked lines (for fast navigation in the log)

    };

    /**
     * Global Singleton Log Manager
     */
    extern LogManager globalLogManager;

} // namespace Berserk

#endif //BERSERKENGINE_LOGMANAGER_H
