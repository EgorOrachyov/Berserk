//
// Created by Egor Orachyov on 24.05.2018.
//

#include "../Core/LogManager.h"
#include "../Core/CompilationFlags.h"

namespace Berserk
{
    LogManager::LogManager()
    {
        mLinesCounter = 0;
        mLoggingLevel = LL_NORMAL;
        mLogFile = fopen("BerserkLogFile.log", "w");
        if (!mLogFile)
        {
            #ifdef DEBUG
                fprintf(stderr, "Cannot open default log file \n");
            #endif

            exit(EXIT_FAILURE);
        }

        InitialMessgae();
    }

    LogManager::~LogManager()
    {
        FinalMessage();
        CloseLogFile();
    }

    void LogManager::SetLoggingFile(const char * fileName)
    {
        FILE * newLog = fopen(fileName, "w");
        if (newLog)
        {
            CloseLogFile();
            mLogFile = newLog;
            mLinesCounter = 0;
        }
        else
        {
            #ifdef DEBUG
                fprintf(stderr, "Cannot open log file %s\n", fileName);
                exit(EXIT_FAILURE);
            #endif
        }

    }


    void LogManager::SetLoggingLevel(LoggingLevel level)
    {
        mLoggingLevel = level;
    }

    void LogManager::PushMessage(LogMessageImportance importance, const char * text)
    {
        if (importance == LMI_ERROR)
        {

            #ifdef DEBUG
                fprintf(stderr, "%s \n", text);
            #endif

            fprintf(mLogFile, "[%i:ERROR] %s \n", mLinesCounter++, text);
        }
        else if (importance == LMI_WARNING)
        {
            #ifdef DEBUG
                fprintf(stdout, "%s \n", text);
            #endif

            fprintf(mLogFile, "[%i:WARNING] %s \n", mLinesCounter++, text);
        }
        else
        {
            #ifdef DEBUG
                fprintf(stdout, "%s \n", text);
            #endif

            fprintf(mLogFile, "[%i:INFO] %s \n", mLinesCounter++, text);
        }
    }

    void LogManager::PushMessage(LogMessageImportance importance, const char * text,
                                 int32 line, const char * file)
    {

    }

    void LogManager::PushMessage(LogMessageImportance importance, const char * text,
                                 int32 line, const char * file, const char * function)
    {

    }

    void LogManager::PushMessage(LogMessageImportance importance, const char * text,
                                 int32 line, const char * file, const char * function, const char * time)
    {
        if (importance == LMI_ERROR)
        {

#ifdef DEBUG
            fprintf(stderr, "%s, line %i, file %s, function %s, %s time \n", text, line, file, function, time);
#endif

            fprintf(mLogFile, "[%i:ERROR] %s, line %i, file %s, function %s, %s time \n",
                    mLinesCounter++, text, line, file, function, time);
        }
        else if (importance == LMI_WARNING)
        {
#ifdef DEBUG
            fprintf(stdout, "%s, line %i, file %s, function %s, %s time \n", text, line, file, function, time);
#endif

            fprintf(mLogFile, "[%i:WARNING] %s, line %i, file %s, function %s, %s time \n",
                    mLinesCounter++, text, line, file, function, time);
        }
        else
        {
#ifdef DEBUG
            fprintf(stdout, "%s, line %i, file %s, function %s, %s time \n", text, line, file, function, time);
#endif

            fprintf(mLogFile, "[%i:INFO] %s, line %i, file %s\n, function %s, %s time \n",
                    mLinesCounter++, text, line, file, function, time);
        }
    }

    void LogManager::CloseLogFile()
    {
        if (mLogFile) {
            fclose(mLogFile);
            mLogFile = NULL;
        }
    }

    void LogManager::InitialMessgae()
    {
        fprintf(mLogFile, "************************** Berserk Engine **************************\n");
        fprintf(mLogFile, "[%i] Log File \n", mLinesCounter++);
        fprintf(mLogFile, "[%i] Build version: %s \n", mLinesCounter++, BUILD_VERSION);
        fprintf(mLogFile, "[%i] Log Manager initialized \n", mLinesCounter++);
        fprintf(mLogFile, "********************************************************************\n\n\n");
    }

    void LogManager::FinalMessage()
    {
        fprintf(mLogFile, "\n\n********************************************************************\n");
        fprintf(mLogFile, "[%i] Log Manager: end writing \n", mLinesCounter++);
        fprintf(mLogFile, "************************** Berserk Engine **************************\n");
    }

    LogManager globalLogManager;

} // namespace Berserk