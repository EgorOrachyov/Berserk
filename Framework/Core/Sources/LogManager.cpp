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
        mTimer.Init();

        // Open default log file
        mLogFile = fopen("BerserkLogFile.log", "w");
        ASSERT(mLogFile, "Cannot open default log file");

        PushInitialMessage();
    }

    LogManager::~LogManager()
    {
        CloseLogFile();
    }

    void LogManager::SetLoggingFile(const char * fileName)
    {
        CloseLogFile();

        mLogFile = fopen(fileName, "w");
        ASSERT(mLogFile, "Cannot open log file %s", fileName);

        mLinesCounter = 0;
        PushInitialMessage();
    }

    void LogManager::SetLoggingFile(StaticStringASCII& fileName)
    {
        CloseLogFile();

        mLogFile = fopen(fileName.GetCharsBuffer(), "w");
        ASSERT(mLogFile, "Cannot open log file %s", fileName.GetCharsBuffer());

        mLinesCounter = 0;
        PushInitialMessage();
    }

    void LogManager::PushInitialMessage()
    {
        fprintf(mLogFile, "----------------------------------- [Berserk Engine] -----------------------------------\n");
        fprintf(mLogFile, "[%i] Log File \n", mLinesCounter++);
        fprintf(mLogFile, "[%i] Build version: %s \n", mLinesCounter++, BUILD_VERSION);
        fprintf(mLogFile, "[%i] Log Manager initialized, time %10.4fs\n", mLinesCounter++, mTimer.GetSeconds());
        fprintf(mLogFile, "----------------------------------------------------------------------------------------\n\n\n");
    }

    void LogManager::PushFinalMessage()
    {
        fprintf(mLogFile, "\n\n----------------------------------------------------------------------------------------\n");
        fprintf(mLogFile, "[%i] Log Manager: end writing, time %10.4fs\n", mLinesCounter++, mTimer.GetSeconds());
        fprintf(mLogFile, "----------------------------------- [Berserk Engine] -----------------------------------\n");
    }

    void LogManager::PushLine()
    {
        fprintf(mLogFile, "----------------------------------------------------------------------------------------\n");
    }

    void LogManager::PushCurrentTime()
    {
        fprintf(mLogFile, "[%i][Time: %10.4fs] \n", mLinesCounter++, mTimer.GetSeconds());
    }

    void LogManager::BeginBlock()
    {
        fprintf(mLogFile, "[%i][Begin block]\n", mLinesCounter++);
    }

    void LogManager::BeginBlock(const CHAR* blockName)
    {
        fprintf(mLogFile, "[%i][Block name: %s]\n", mLinesCounter++, blockName);
    }

    void LogManager::BeginBlock(StaticStringASCII& blockName)
    {
        fprintf(mLogFile, "[%i][Block name: %s]\n", mLinesCounter++, blockName.GetCharsBuffer());

    }

    void LogManager::EndBlock()
    {
        fprintf(mLogFile, "[%i][End block block]\n", mLinesCounter++);
    }

    void LogManager::PushFreeLine()
    {
        fprintf(mLogFile, "\n");
    }

    void LogManager::PushMessage(LogMessageType type, const CHAR* MSG)
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

    void LogManager::PushMessage(LogMessageType type, StaticStringASCII& MSG)
    {
        if (type == LogMessageType::LMT_ERROR)
        {
            #if DEBUG
                fprintf(stderr, "[%i][ERROR] %s\n", mLinesCounter, MSG.GetCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][ERROR] %s\n", mLinesCounter++, MSG.GetCharsBuffer());
        }
        else if (type == LogMessageType::LMT_WARNING)
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, MSG.GetCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, MSG.GetCharsBuffer());
        }
        else if (type == LogMessageType::LMT_INFO)
        {
            #if DEBUG
                fprintf(stdout, "[%i][INFO] %s\n", mLinesCounter, MSG.GetCharsBuffer());
            #endif

            fprintf(mLogFile, "[%i][INFO] %s\n", mLinesCounter++, MSG.GetCharsBuffer());
        }
        else
        {
            #if DEBUG
                fprintf(stdout, "[%i][WARNING] %s\n", mLinesCounter, "Unknown log message type");
            #endif

            fprintf(mLogFile, "[%i][WARNING] %s\n", mLinesCounter++, "Unknown log message type");
        }
    }

    void LogManager::PushMessageBlock(const CHAR* MSG)
    {
        fprintf(mLogFile, "%s\n", MSG);
    }

    void LogManager::PushMessageBlock(StaticStringASCII& MSG)
    {
        fprintf(mLogFile, "%s\n", MSG.GetCharsBuffer());
    }

    int64 LogManager::GetCurrentLineNumber()
    {
        return mLinesCounter;
    }

    float64 LogManager::GetCurrentTime()
    {
        return mTimer.GetSeconds();
    }

    void LogManager::CloseLogFile()
    {
        PushFinalMessage();
        fclose(mLogFile);
    }

    void LogManager::ConsoleWriteError(const CHAR* MSG)
    {
        fprintf(stderr, "%s", MSG);
    }

    void LogManager::ConsoleWriteInfo(const CHAR* MSG)
    {
        fprintf(stdout, "%s", MSG);
    }

    LogManager& LogManager::GetGlobalLogManager()
    {
        return globalLogManager;
    }

    LogManager globalLogManager;

} // namespace Berserk