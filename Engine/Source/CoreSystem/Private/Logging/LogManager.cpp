//
// Created by Egor Orachyov on 02.07.2019.
//

#include <Logging/LogManager.h>

namespace Berserk
{

    LogManager::LogManager(IFile &file, IOutputDevice &device, ELogVerbosity verbosity)
            : mFile(file), mVerbosity(verbosity), mDevice(device)
    {
        addMessageInit();
        mFile.flush();
    }

    LogManager::~LogManager()
    {
        addMessageFinal();
        mFile.flush();
    }

    void LogManager::addMessage(const char *message, ELogVerbosity verbosity, bool mirrorToOutput)
    {
        CriticalSection section(mMutex);

        if (mVerbosity > verbosity || mVerbosity == NoLogging)
        { return; }

        char verbosityStr[Buffers::SIZE_64];
        writeVerbosity(verbosity, verbosityStr);

        char buffer[WRITE_MESSAGE_SIZE];
        int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE, "[%s][%li][%s] %s\n",
                mClock.update().toString().get(), mMessagesNum, verbosityStr, message);

        mMessagesNum += 1;
        writeToFile(written, buffer);

        if (mirrorToOutput)
        {
            mDevice.print(buffer);
        }
    }

    void LogManager::addMessage(const char *category, const char *message, ELogVerbosity verbosity,
                                bool mirrorToOutput)
    {
        CriticalSection section(mMutex);

        if (mVerbosity > verbosity || mVerbosity == NoLogging)
        { return; }

        char verbosityStr[Buffers::SIZE_64];
        writeVerbosity(verbosity, verbosityStr);

        char buffer[WRITE_MESSAGE_SIZE];
        int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE, "[%s][%li][%s] (%s) %s\n",
                mClock.update().toString().get(), mMessagesNum, verbosityStr, category, message);

        mMessagesNum += 1;
        writeToFile(written, buffer);

        if (mirrorToOutput)
        {
            mDevice.print(buffer);
        }
    }

    void LogManager::addPage()
    {
        CriticalSection section(mMutex);

        char buffer[WRITE_MESSAGE_SIZE];
        int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
                                       "\n------------------------------------------"
                                       "[Page %lu]"
                                       "------------------------------------------\n\n",
                                       mPageNum);

        mPageNum += 1;
        writeToFile(written, buffer);
    }

    void LogManager::addMessageInit()
    {
        CriticalSection section(mMutex);

        char buffer[WRITE_MESSAGE_SIZE];
        int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
                                       "------------------------------------------[Berserk Engine]------------------------------------------\n\n");

        writeToFile(written, buffer);
    }

    void LogManager::addMessageFinal()
    {
        CriticalSection section(mMutex);

        char buffer[WRITE_MESSAGE_SIZE];
        int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
                                       "\n------------------------------------------[Berserk Engine]------------------------------------------\n");

        writeToFile(written, buffer);
    }

    void LogManager::writeVerbosity(ELogVerbosity verbosity, char *buffer)
    {
        switch (verbosity)
        {
            case NoLogging:
                buffer[0] = '\0';
                break;

            case Display:
                sprintf(buffer, "Display");
                break;

            case Warning:
                sprintf(buffer, "Warning");
                break;

            case Error:
                sprintf(buffer, "Error");
                break;

            case Fatal:
                sprintf(buffer, "Fatal");
                break;

            default:
                throw Exception("LogManager: unknown message verbosity level");
        }
    }

    void LogManager::writeToFile(int32 written, char *buffer)
    {
        if (written > 0) mFile.write(buffer, written);
        else throw Exception("LogManager: cannot write to log");
    }

} // namespace Berserk