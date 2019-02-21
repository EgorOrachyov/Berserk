//
// Created by Egor Orachyov on 24.01.2019.
//

#include "Time/Timer.h"
#include "Info/Version.h"
#include "Misc/Compilation.h"
#include "Logging/LogManager.h"

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

        mWriteToFile = true;
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
            if (mWriteToFile) flush(buffer3);

            mLinesCount += 1;
            mMessagesCount += 1;
        }
    }

    void LogManager::push(LogVerbosity type, const char *message)
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

            sprintf(buffer3, "[%u][%s]\n%s\n", mLinesCount, messageType.get(), message);

#if DEBUG
            if (type <= LogVerbosity::Error) fprintf(stderr, "%s", buffer3);
            else fprintf(stdout, "%s", buffer3);
#else
            if (type == LogVerbosity::Fatal) fprintf(stderr, "%s", buffer3);
#endif
            if (mWriteToFile) flush(buffer3);

            mLinesCount += 1;
            mMessagesCount += 1;
        }
    }

    void LogManager::addLine()
    {
#if DEBUG
        fprintf(stdout, "----------------------------------------------------------------------------------------------------\n");
#endif
        flush("----------------------------------------------------------------------------------------------------\n");
    }

    void LogManager::addEmptyLine()
    {
#if DEBUG
        fprintf(stdout, "\n");
#endif
        if (mWriteToFile) flush("\n");
    }

    void LogManager::addHeader(const char *title)
    {
        char line[] = "---------------------------------------------------------------------------------------------------\n";
        auto length = Utils::strlen(title);
        auto buffer = (char*)buffer3;

        if (length >= MAX_MESSAGE_SIZE)
        {
            fprintf(stderr, "Log Manager: Fatal: log header title is too big '%s'\n", title);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "\n[%u][%s]", mLinesCount++, title);
        Utils::strncat(buffer, line, LINE_LENGTH + 1);
        Utils::strcat(buffer, "\n\n");

#if DEBUG
        fprintf(stdout, buffer);
#endif
        if (mWriteToFile) flush(buffer);
    }

    void LogManager::setVerbosity(LogVerbosity verbosity)
    {
        mLogVerbosity = verbosity;
    }

    void LogManager::setFileWriting(bool write)
    {
        mWriteToFile = write;
    }

    void LogManager::blockOpen(const char *title)
    {
        auto length = Utils::strlen(title);
        auto buffer = (char*)buffer3;

        if (length >= MAX_MESSAGE_SIZE)
        {
            fprintf(stderr, "Log Manager: Fatal: log block title is too big '%s'\n", title);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "[%u][%s]\n", mLinesCount++, title);
#if DEBUG
        fprintf(stdout, buffer);
#endif
        mLinesCount += 1;
        mMessagesCount += 1;

        if (mWriteToFile) flush(buffer);
    }

    void LogManager::blockPush(const char *message)
    {
        auto length = Utils::strlen(message);
        auto buffer = (char*)buffer3;

        if (length >= MAX_MESSAGE_SIZE)
        {
            fprintf(stderr, "Log Manager: Fatal: log message is too big '%s'\n", message);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "%s\n", message);
#if DEBUG
        fprintf(stdout, buffer);
#endif
        mLinesCount += 1;
        mMessagesCount += 1;

        if (mWriteToFile) flush(buffer);
    }

    void LogManager::blockClose(const char *title)
    {
        auto length = Utils::strlen(title);
        auto buffer = (char*)buffer3;

        if (length >= MAX_MESSAGE_SIZE)
        {
            fprintf(stderr, "Log Manager: Fatal: log block title is too big '%s'\n", title);
            exit(EXIT_FAILURE);
        }

        sprintf(buffer, "[%u][%s]\n", mLinesCount++, title);
#if DEBUG
        fprintf(stdout, buffer);
#endif
        mLinesCount += 1;
        mMessagesCount += 1;

        if (mWriteToFile) flush(buffer);
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
        fprintf(mLogFile, "------------------------------------------[Berserk Engine]------------------------------------------\n");
        fprintf(mLogFile, "[%u] Log File \n", mLinesCount++);
        fprintf(mLogFile, "[%u] Time start: %lfs \n", mLinesCount++, Timer::getSingleton().current());
        fprintf(mLogFile, "[%u] Engine Version: %i.%i\n", mLinesCount++, BERSERK_VERSION_MAJOR, BERSERK_VERSION_MINOR);
        fprintf(mLogFile, "------------------------------------------[Berserk Engine]------------------------------------------\n\n\n");
    }

    void LogManager::endLog()
    {
        fprintf(mLogFile, "\n\n------------------------------------------[Berserk Engine]------------------------------------------\n");
        fprintf(mLogFile, "[%u] Log File \n", mLinesCount++);
        fprintf(mLogFile, "[%u] Time total: %lfs \n", mLinesCount++, Timer::getSingleton().current());
        fprintf(mLogFile, "[%u] Engine Version: %i.%i\n", mLinesCount++, BERSERK_VERSION_MAJOR, BERSERK_VERSION_MINOR);
        fprintf(mLogFile, "[%u] Total messages count: %u \n", mLinesCount++, mMessagesCount);
        fprintf(mLogFile, "------------------------------------------[Berserk Engine]------------------------------------------\n");
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