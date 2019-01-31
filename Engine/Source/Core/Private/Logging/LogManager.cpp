//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Public/Logging/LogManager.h"
#include "Public/Info/Version.h"
#include "Public/Misc/Compilation.h"

namespace Berserk
{

    LogManager::LogManager(const char *filename)
    {
        mLogFile = fopen(filename,"w");
        if (mLogFile == nullptr)
        {
            fprintf(stderr, "Log Manager: Fatal: Cannot open log file with name '%s'\n", filename);
            exit(EXIT_FAILURE);
        }

        mStreamLength = 0;
        mLinesCount = 0;
        mMessagesCount = 0;
        mLogVerbosity = LogVerbosity::Display;

        beginLog();
    }

    LogManager::~LogManager()
    {
        explicitClose();
    }

    void LogManager::addMessage(LogVerbosity type, const char *message)
    {
        if (type <= mLogVerbosity)
        {
            auto length = Utils::strlen(message);

            if (length >= MAX_MESSAGE_SIZE)
            {
                fprintf(stderr, "Log Manager: Fatal: log message is too big '%s'\n", message);
                exit(EXIT_FAILURE);
            }

            StringStream<char,'\0',16> messageType;

            switch (type)
            {
                case LogVerbosity::Fatal :
                    messageType += "Fatal";
                    break;

                case LogVerbosity::Error :
                    messageType += "Error";
                    break;

                case LogVerbosity::Warning :
                    messageType += "Warning";
                    break;

                case LogVerbosity::Display :
                    messageType += "Display";
                    break;

                default:
                    fprintf(stderr, "Log Manager: Fatal: incorrect log type in message '%s'\n", message);
                    exit(EXIT_FAILURE);
            }

            sprintf(buffer3, "[%u][%s] %s\n", mLinesCount, messageType.get(), message);

#if DEBUG
            if (type <= LogVerbosity::Error) fprintf(stderr, "%s", buffer3);
            else fprintf(stdout, "%s", buffer3);
#else
            if (type == LogVerbosity::Fatal) fprintf(stderr, "%s", buffer3);
#endif
            flush(buffer3);

            mLinesCount += 1;
            mMessagesCount += 1;
        }
    }

    void LogManager::flush(const char *buffer)
    {
        auto length = Utils::strlen(buffer);

        if (length + mStreamLength >= LOG_BUFFER_SIZE)
        {
            fprintf(mLogFile, "%s%s", mStream.get(), buffer);
            mStream.empty();
            mStreamLength = 0;
        }
        else
        {
            mStream += buffer;
            mStreamLength += length;
        }
    }

    void LogManager::beginLog()
    {
        fprintf(mLogFile, "---------------------------------------[Berserk Engine]---------------------------------------\n");
        fprintf(mLogFile, "[%u] Log File \n", mLinesCount++);
        fprintf(mLogFile, "[%u] Engine Version: %i.%i\n", mLinesCount++,BERSERK_VERSION_MAJOR, BERSERK_VERSION_MINOR);
        fprintf(mLogFile, "---------------------------------------[Berserk Engine]---------------------------------------\n\n\n");
    }

    void LogManager::endLog()
    {
        fprintf(mLogFile, "\n\n---------------------------------------[Berserk Engine]---------------------------------------\n");
        fprintf(mLogFile, "[%u] Log File \n", mLinesCount++);
        fprintf(mLogFile, "[%u] Engine Version: %i.%i\n", mLinesCount++,BERSERK_VERSION_MAJOR, BERSERK_VERSION_MINOR);
        fprintf(mLogFile, "[%u] Total messages count: %u \n", mLinesCount++, mMessagesCount);
        fprintf(mLogFile, "---------------------------------------[Berserk Engine]---------------------------------------\n");
        fflush(mLogFile);
    }

    void LogManager::explicitClose()
    {
        if (mLogFile)
        {
            fprintf(mLogFile, "%s", mStream.get());
            endLog();
            fclose(mLogFile);
            mLogFile = nullptr;
        }
    }

    LogManager& LogManager::getSingleton()
    {
        static LogManager logManager("Berserk.log");
        return logManager;
    }

}