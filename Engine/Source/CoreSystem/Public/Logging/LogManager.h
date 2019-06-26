//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include <IO/IFile.h>
#include <Misc/Buffers.h>
#include <Threading/Mutex.h>
#include <Memory/Allocator.h>
#include <IO/IOutputDevice.h>
#include <Logging/ILogManager.h>

namespace Berserk
{

    /**
     * Default log manager implementation with internal buffer for
     * temporal tasks. Should be used only from one thread.
     *
     * @note Multi-threaded, Synchronized
     */
    class CORE_API LogManager : public ILogManager
    {
    public:

        /**
         * Create log manager and push init message
         * @param file Handler of file to write the log
         * @param verbosity Verbosity of this log to filter messages
         */
        explicit LogManager(IFile &file, IOutputDevice& device, ELogVerbosity verbosity = ELogVerbosity::Display)
                : mFile(file), mVerbosity(verbosity), mDevice(device)
        {
            addMessageInit();
            mFile.flush();
        }

        ~LogManager()
        {
            addMessageFinal();
            mFile.flush();
        }

        void addMessage(const char *message, ELogVerbosity verbosity, bool mirrorToOutput) override
        {
            Guard guard(mMutex);

            if (mVerbosity > verbosity || mVerbosity == NoLogging)
            { return; }

            char verbosityStr[Buffers::SIZE_64];
            writeVerbosity(verbosity, verbosityStr);

            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE, "[%li][%s] %s \n", mMessagesNum, verbosityStr, message);

            mMessagesNum += 1;
            writeToFile(written, buffer);

            if (mirrorToOutput)
            {
                mDevice.print(buffer);
            }
        }

        void addMessage(const char *category, const char *message,
                        ELogVerbosity verbosity, bool mirrorToOutput) override
        {
            Guard guard(mMutex);

            if (mVerbosity > verbosity || mVerbosity == NoLogging)
            { return; }

            char verbosityStr[Buffers::SIZE_64];
            writeVerbosity(verbosity, verbosityStr);

            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE, "[%li][%s] (%s) %s \n", mMessagesNum, verbosityStr, category, message);

            mMessagesNum += 1;
            writeToFile(written, buffer);

            if (mirrorToOutput)
            {
                mDevice.print(buffer);
            }
        }

        void addPage() override
        {
            Guard guard(mMutex);

            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
            "\n------------------------------------------"
              "[Page %lu]"
              "------------------------------------------\n\n",
            mPageNum);

            mPageNum += 1;
            writeToFile(written, buffer);
        }

        ELogVerbosity getVerbosity() const override
        {
            return mVerbosity;
        }

    protected:

        void addMessageInit()
        {
            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
            "------------------------------------------[Berserk Engine]------------------------------------------\n\n");

            writeToFile(written, buffer);
        }

        void addMessageFinal()
        {
            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = Printer::print(buffer, WRITE_MESSAGE_SIZE,
            "\n------------------------------------------[Berserk Engine]------------------------------------------\n");

            writeToFile(written, buffer);
        }

        void writeVerbosity(ELogVerbosity verbosity, char* buffer)
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
                    sprintf(buffer, "Display");
                    break;

                case Error:
                    sprintf(buffer, "Display");
                    break;

                case Fatal:
                    sprintf(buffer, "Display");
                    break;

                default:
                   throw Exception("LogManager: unknown message verbosity level");
            }
        }

        void writeToFile(int32 written, char* buffer)
        {
            if (written > 0) mFile.write(buffer, written);
            else throw Exception("LogManager: cannot write to log");
        }

    protected:

        volatile ELogVerbosity mVerbosity;
        IFile& mFile;
        IOutputDevice& mDevice;
        uint64 mMessagesNum = 0;
        uint64 mPageNum = 0;
        Mutex mMutex;

    };

} // namespace Berserk

#endif //BERSERK_LOGMANAGER_H