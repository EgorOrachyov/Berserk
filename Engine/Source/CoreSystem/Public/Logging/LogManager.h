//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include <IO/IFile.h>
#include <Misc/Buffers.h>
#include <Exception/CoreException.h>
#include <Memory/Allocator.h>
#include <Logging/ILogManager.h>
#include <Strings/StringStream.h>

namespace Berserk
{

    /**
     * Default log manager implementation with internal buffer for
     * temporal tasks. Should be used only from one thread.
     *
     * @note Single-threaded
     */
    class CORE_API LogManager : public ILogManager {
    public:

        /** Char string */
        typedef Strings<char, '\0'> Utils;

        /** Allows to capture stack memory to uses formatted print in buffer*/
        static const uint32 WRITE_BUFFER_SIZE = Buffers::KiB;

        /** Buffer to format message for custom log output  */
        static const uint32 WRITE_MESSAGE_SIZE = Buffers::KiB;

    public:

        explicit LogManager(IFile &file, ELogVerbosity verbosity = ELogVerbosity::Error)
                : mFile(file), mVerbosity(verbosity)
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
            if (mVerbosity > verbosity || mVerbosity == NoLogging)
            { return; }

            char verbosityStr[Buffers::SIZE_64];
            writeVerbosity(verbosity, verbosityStr);

            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = snprintf(buffer, WRITE_MESSAGE_SIZE, "[%li][%s] %s \n", mMessagesNum++, verbosityStr, message);

            writeToFile(written, buffer);

            if (mirrorToOutput)
            {
                // todo
            }
        }

        void addMessage(const char *category, const char *message,
                        ELogVerbosity verbosity, bool mirrorToOutput) override
        {
            if (mVerbosity > verbosity || mVerbosity == NoLogging)
            { return; }

            char verbosityStr[Buffers::SIZE_64];
            writeVerbosity(verbosity, verbosityStr);

            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = snprintf(buffer, WRITE_MESSAGE_SIZE, "[%li][%s] (%s) %s \n", mMessagesNum++, verbosityStr, category, message);

            writeToFile(written, buffer);

            if (mirrorToOutput)
            {
                // todo
            }
        }

        template <typename ... TArgs>
        void addMessage(ELogVerbosity verbosity, bool mirrorToOutput,
                        const char *format, const TArgs& ... args)
        {
            char buffer[WRITE_BUFFER_SIZE];
            uint32 written = 0;

            addMessageImp(buffer, written, 0, format, args...);
            addMessage(buffer, verbosity, mirrorToOutput);
        }

        void addPage() override
        {
            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = snprintf(buffer, WRITE_MESSAGE_SIZE,
            "\n------------------------------------------"
              "[Page %lu]"
              "------------------------------------------\n\n",
            mPageNum++);

            writeToFile(written, buffer);
        }

        ELogVerbosity getVerbosity() const override
        {
            return mVerbosity;
        }

    protected:

        template<typename TArg>
        void addMessageImp(char *buffer, uint32 written, uint32 pos, const char *format, const TArg &arg)
        {
            char write[Buffers::SIZE_256];
            char mask[Buffers::SIZE_256];

            uint32 i = pos;
            while (written < WRITE_BUFFER_SIZE - 1 && format[i] != '\0')
            {
                if (format[i] == '%')
                {
                    uint32 size = 0;
                    while (format[i] != '\0' && format[i] != ' ')
                    {
                        mask[size++] = format[i++];
                    }
                    mask[size] = '\0';
                    int32 result = snprintf(write, SIZE_256, mask, arg);

                    if (result > 0)
                    {
                        char* towrite = write;
                        while ((written < WRITE_BUFFER_SIZE - 1) && *towrite != '\0')
                        {
                            buffer[written++] = *towrite;
                            towrite += 1;
                        }
                    }
                    else
                    {
                        throw CoreException("LogManager: invalid file mask");
                    }
                }
                else
                {
                    buffer[written++] = format[i++];
                }
            }

            /** After that function no more printing in buffer */
            buffer[written] = '\0';
        }

        template <typename TArg, typename ... TArgs>
        void addMessageImp(char* buffer, uint32 written, uint32 pos, const char* format, const TArg& arg, const TArgs& ... args)
        {
            char write[Buffers::SIZE_256];
            char mask[Buffers::SIZE_256];

            uint32 i = pos;
            while (written < WRITE_BUFFER_SIZE - 1 && format[i] != '\0')
            {
                if (format[i] == '%')
                {
                    uint32 size = 0;
                    while (format[i] != '\0' && format[i] != ' ')
                    {
                        mask[size++] = format[i++];
                    }
                    mask[size] = '\0';
                    int32 result = snprintf(write, SIZE_256, mask, arg);

                    if (result > 0)
                    {
                        char* towrite = write;
                        while (written < WRITE_BUFFER_SIZE - 1 && *towrite != '\0')
                        {
                            buffer[written++] = *towrite;
                            towrite += 1;
                        }
                    }
                    else
                    {
                        throw CoreException("LogManager: invalid file mask");
                    }

                    addMessageImp(buffer, written, i, format, args...);
                    return;
                }
                else
                {
                    buffer[written++] = format[i++];
                }
            }
        }

        void addMessageInit()
        {
            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = snprintf(buffer, WRITE_MESSAGE_SIZE,
            "------------------------------------------[Berserk Engine]------------------------------------------\n\n");

            writeToFile(written, buffer);
        }

        void addMessageFinal()
        {
            char buffer[WRITE_MESSAGE_SIZE];
            int32 written = snprintf(buffer, WRITE_MESSAGE_SIZE,
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
                   throw CoreException("LogManager: unknown message verbosity level");
            }
        }

        void writeToFile(int32 written, char* buffer)
        {
            if (written > 0) mFile.write(buffer, written);
            else throw CoreException("LogManager: cannot write to log");
        }

    protected:

        ELogVerbosity mVerbosity;
        IFile& mFile;
        uint64 mMessagesNum = 0;
        uint64 mPageNum = 0;

    };

} // namespace Berserk

#endif //BERSERK_LOGMANAGER_H