//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Logging/LogManager.h"

namespace Berserk
{

    LogManager::LogManager(const char *logFileName)
    {
        mLogFile = fopen(logFileName,"w");
        if (mLogFile == nullptr)
        {
            fprintf(stderr, "Fatal: Cannot open log file with name '%s'\n", logFileName);
            exit(1);
        }

        mLinesCount = 0;
        mMessagesCount = 0;
    }

    LogManager::~LogManager()
    {
        if (mLogFile)
        {
            flush();
            fclose(mLogFile);
        }
    }

    void LogManager::addMessage(LogVerbosity verbosity, const char *message)
    {

    }

    void LogManager::flush()
    {

    }

    void LogManager::beginLog()
    {

    }

    void LogManager::endLog()
    {

    }

    LogManager& LogManager::getSingleton()
    {
        static LogManager logManager("Berserk.log");
        return logManager;
    }

}