//
// Created by Egor Orachyov on 24.05.2018.
//

#ifndef BERSERKENGINE_LOGMANAGER_H
#define BERSERKENGINE_LOGMANAGER_H

#include "Common.h"
#include "Types.h"

namespace Berserk
{
    /**
     *  @brief Message importance level
     */
    enum LogMessageImportance
    {
        LMI_INFO = 1,
        LMI_WARNING = 2,
        LMI_ERROR = 3
    };

    /**
     *  @brief Level of logging (amount of addition info)
     */
    enum LoggingLevel
    {
        LL_LOW = 1,
        LL_NORMAL = 2,
        LL_HIGH = 3
    };

    /**
     *  Singleton log manager for pushing messages into log file
     *  and console (if debug mode)
     */
    class LogManager
    {
    public:

        LogManager();

        ~LogManager();

        /**
         *
         * @param fileName Name of desired log file (it will be created and writed)
         */
        void SetLoggingFile(const char * fileName);

        /**
         *
         * @param level Level of logging @see LoggingLevel
         */
        void SetLoggingLevel(LoggingLevel level);

        /**
         *
         * @param importance Message importance @see LogMessageImportance
         * @param text String message text
         */
        void PushMessage(LogMessageImportance importance, const char *text);

        /**
         *
         * @param importance Message importance @see LogMessageImportance
         * @param text String message text
         * @param line Line of file
         * @param file Name of file
         */
        void PushMessage(LogMessageImportance importance, const char * text,
                         int32 line, const char * file);

        /**
         *
         * @param importance Message importance @see LogMessageImportance
         * @param text String message text
         * @param line Line of file
         * @param file Name of file
         * @param function Name of function
         */
        void PushMessage(LogMessageImportance importance, const char * text,
                         int32 line, const char * file, const char * function);

        /**
         *
         * @param importance Message importance @see LogMessageImportance
         * @param text String message text
         * @param line Line of file
         * @param file Name of file
         * @param function Name of function
         * @param time Time of invocation
         */
        void PushMessage(LogMessageImportance importance, const char * text,
                         int32 line, const char * file, const char * function, const char * time);

    private:

        void CloseLogFile();

        void InitialMessgae();

        void FinalMessage();

    private:

        FILE * mLogFile;
        LoggingLevel mLoggingLevel;
        int32 mLinesCounter;

    };

    /**
     * Global Singleton Log Manager
     */
    extern LogManager globalLogManager;

} // namespace Berserk

#endif //BERSERKENGINE_LOGMANAGER_H
