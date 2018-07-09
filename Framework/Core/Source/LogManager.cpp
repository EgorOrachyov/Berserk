//
// Created by Egor Orachyov on 24.05.2018.
//

#include "../Logging/LogManager.h"
#include "../Essential/CompilationFlags.h"
#include "../Essential/Assert.h"

namespace Berserk
{
    LogManager::LogManager()
    {
        mLinesCounter = 0;
        mLogFile = NULL;
        mTimer.init();

        // Open default log file
        mLogFile = fopen("Berserk.log", "w");
        ASSERT(mLogFile, "Cannot open default log file");

        pushInitialMessage();
    }

    LogManager::~LogManager()
    {
        closeLogFile();
    }

    void LogManager::setLoggingFile(const char *fileName)
    {
        closeLogFile();

        mLogFile = fopen(fileName, "w");
        ASSERT(mLogFile, "Cannot open log file %s", fileName);

        mLinesCounter = 0;
        pushInitialMessage();
    }

    void LogManager::setLoggingFile(CHARStaticString &fileName)
    {
        closeLogFile();

        mLogFile = fopen(fileName.getCharsBuffer(), "w");
        ASSERT(mLogFile, "Cannot open log file %s", fileName.getCharsBuffer());

        mLinesCounter = 0;
        pushInitialMessage();
    }

    void LogManager::pushInitialMessage()
    {
        fprintf(mLogFile, "----------------------------------- [Berserk Engine] -----------------------------------\n");
        fprintf(mLogFile, "[%i] Log File \n", mLinesCounter++);
        fprintf(mLogFile, "[%i] Build version: %s \n", mLinesCounter++, BUILD_VERSION);
        fprintf(mLogFile, "[%i] Log Manager initialized, time %10.4fs\n", mLinesCounter++, mTimer.getSeconds());
        fprintf(mLogFile, "----------------------------------------------------------------------------------------\n\n\n");
    }

    void LogManager::pushFinalMessage()
    {
        fprintf(mLogFile, "\n\n----------------------------------------------------------------------------------------\n");
        fprintf(mLogFile, "[%i] Log Manager: end writing, time %10.4fs\n", mLinesCounter++, mTimer.getSeconds());
        fprintf(mLogFile, "----------------------------------- [Berserk Engine] -----------------------------------\n");
    }

    void LogManager::pushLine()
    {
        fprintf(mLogFile, "----------------------------------------------------------------------------------------\n");
    }

    void LogManager::pushCurrentTime()
    {
        #if DEBUG
            fprintf(stdout, "[%i][Time: %10.4fs] \n", mLinesCounter, mTimer.getSeconds());
        #endif
        fprintf(mLogFile, "[%i][Time: %10.4fs] \n", mLinesCounter++, mTimer.getSeconds());
    }

    void LogManager::beginBlock()
    {
        #if DEBUG
            fprintf(stdout, "[%i][Begin block]\n", mLinesCounter);
        #endif
        fprintf(mLogFile, "[%i][Begin block]\n", mLinesCounter++);
    }

    void LogManager::beginBlock(const CHAR *blockName)
    {
        #if DEBUG
            fprintf(stdout, "[%i][Block name: %s]\n", mLinesCounter, blockName);
        #endif
        fprintf(mLogFile, "[%i][Block name: %s]\n", mLinesCounter++, blockName);
    }

    void LogManager::beginBlock(CHARStaticString &blockName)
    {
        #if DEBUG
            fprintf(stdout, "[%i][Block name: %s]\n", mLinesCounter, blockName.getCharsBuffer());
        #endif
        fprintf(mLogFile, "[%i][Block name: %s]\n", mLinesCounter++, blockName.getCharsBuffer());
    }

    void LogManager::endBlock()
    {
        #if DEBUG
            fprintf(stdout, "[%i][End block]\n", mLinesCounter);
        #endif
        fprintf(mLogFile, "[%i][End block]\n", mLinesCounter++);
    }

    void LogManager::pushFreeLine()
    {
        #if DEBUG
            fprintf(stdout, "\n");
        #endif
        fprintf(mLogFile, "\n");
    }

    void LogManager::pushMessage(LogMessageType type, const CHAR *MSG)
    {
        if (type == LogMessageType::LMT_ERROR)
        {
            #if DEBUG
                fprintf(stderr, "[%i][ERROR] %s\n", mLinesCounter, MSG);
            #endif

            fprintf(mLogFile, "[%i][ERROR] %s\n", mLinesCounter++, MSG);
        }
        else if (type == LogMessageType::LMT_WARNING)
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, MSG);
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, MSG);
        }
        else if (type == LogMessageType::LMT_INFO)
        {
            #if DEBUG
                fprintf(stdout, "[%i][INFO] %s\n", mLinesCounter, MSG);
            #endif

            fprintf(mLogFile, "[%i][INFO] %s\n", mLinesCounter++, MSG);
        }
        else
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, "Unknown log message type");
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, "Unknown log message type");
        }
    }

    void LogManager::pushMessage(LogMessageType type, CHARStaticString &MSG)
    {
        if (type == LogMessageType::LMT_ERROR)
        {
            #if DEBUG
                fprintf(stderr, "[%i][ERROR] %s\n", mLinesCounter, MSG.getCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][ERROR] %s\n", mLinesCounter++, MSG.getCharsBuffer());
        }
        else if (type == LogMessageType::LMT_WARNING)
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, MSG.getCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, MSG.getCharsBuffer());
        }
        else if (type == LogMessageType::LMT_INFO)
        {
            #if DEBUG
                fprintf(stdout, "[%i][INFO] %s\n", mLinesCounter, MSG.getCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][INFO] %s\n", mLinesCounter++, MSG.getCharsBuffer());
        }
        else
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, "Unknown log message type");
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, "Unknown log message type");
        }
    }

    void LogManager::pushMessageBlock(const CHAR *MSG)
    {
        #if DEBUG
            fprintf(stdout, "%s\n", MSG);
        #endif
        fprintf(mLogFile, "%s\n", MSG);
    }

    void LogManager::pushMessageBlock(CHARStaticString &MSG)
    {
        #if DEBUG
            fprintf(stdout, "%s\n", MSG.getCharsBuffer());
        #endif
        fprintf(mLogFile, "%s\n", MSG.getCharsBuffer());
    }

    INT64 LogManager::getCurrentLineNumber()
    {
        return mLinesCounter;
    }

    FLOAT64 LogManager::getCurrentTime()
    {
        return mTimer.getSeconds();
    }

    void LogManager::closeLogFile()
    {
        pushFinalMessage();
        fclose(mLogFile);
    }

    void LogManager::consoleWriteError(const CHAR *MSG)
    {
        fprintf(stderr, "%s", MSG);
    }

    void LogManager::consoleWriteInfo(const CHAR *MSG)
    {
        fprintf(stdout, "%s", MSG);
    }

    LogManager& LogManager::getGlobalLogManager()
    {
        return globalLogManager;
    }

    LogManager globalLogManager;

} // namespace Berserk