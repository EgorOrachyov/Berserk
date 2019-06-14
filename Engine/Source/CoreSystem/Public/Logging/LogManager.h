//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include <IO/IFile.h>
#include <Misc/Buffers.h>
#include <Memory/Allocator.h>
#include <Logging/ILogManager.h>
#include <Strings/StringStream.h>
#include <exception>

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

        class InvalidMaskException : public std::exception
        {
        public:
            const char *what() const noexcept override
            { return "Invalid format mask param"; }
        };


        /** Char string */
        typedef Strings<char, '\0'> Utils;

        /** Allows to capture stack memory to uses formated print in buffer*/
        static const uint32 WRITE_BUFFER_SIZE = Buffers::KiB;

    public:

        explicit LogManager(IFile &file, ELogVerbosity verbosity = ELogVerbosity::Error)
                : mFile(file), mVerbosity(verbosity) {

        }

        ~LogManager() {

        }

        void addMessage(const char *message, ELogVerbosity verbosity, bool mirrorToOutput) override {
            if (mVerbosity > verbosity) {
                return;
            }


        }

        void addMessage(const char *category, const char *message,
                        ELogVerbosity verbosity, bool mirrorToOutput) override {

        }

        template <typename ... TArgs>
        void addMessage(ELogVerbosity verbosity, bool mirrorToOutput,
                        const char *format, const TArgs& ... args)
        {
            char buffer[WRITE_BUFFER_SIZE];
            uint32 written = 0;

            addMessageImp(buffer, written, 0, format, args...);
            printf("LogManager: %s\n", buffer);
        }

        void addPage() override {

        }

        ELogVerbosity getVerbosity() const override {
            return mVerbosity;
        }

    protected:

        template<typename TArg>
        void addMessageImp(char *buffer, uint32 written, uint32 pos, const char *format, const TArg &arg)
        {
            char write[Buffers::SIZE_256];
            char mask[Buffers::SIZE_256];

            for (uint32 i = pos; written < WRITE_BUFFER_SIZE - 1 && format[i] != '\0'; i++)
            {
                if (format[i] == '%')
                {
                    uint32 size = 0;
                    while (format[i] != '\0' && format[i] != ' ')
                    {
                        mask[size++] = format[i++];
                    }
                    mask[size] = '\0';
                    int32 result = sprintf(write, mask, arg);

                    if (result > 0)
                    {
                        uint32 towrite = 0;
                        while (written < WRITE_BUFFER_SIZE - 1 && write[towrite] != '\0')
                        {
                            buffer[written++] = write[towrite++];
                        }
                    }
                    else
                    {
                        throw InvalidMaskException();
                    }
                }
                else
                {
                    buffer[written++] = format[i];
                }
            }
        }

        template <typename TArg, typename ... TArgs>
        void addMessageImp(char* buffer, uint32 written, uint32 pos, const char* format, const TArg& arg, const TArgs& ... args)
        {
            char write[Buffers::SIZE_256];
            char mask[Buffers::SIZE_256];

            for (uint32 i = pos; written < WRITE_BUFFER_SIZE - 1 && format[i] != '\0'; i++)
            {
                if (format[i] == '%')
                {
                    uint32 size = 0;
                    while (format[i] != '\0' && format[i] != ' ')
                    {
                        mask[size++] = format[i++];
                    }
                    mask[size] = '\0';
                    int32 result = sprintf(write, mask, arg);

                    if (result > 0)
                    {
                        uint32 towrite = 0;
                        while (written < WRITE_BUFFER_SIZE - 1 && write[towrite] != '\0')
                        {
                            buffer[written++] = write[towrite++];
                        }
                    }
                    else
                    {
                        throw InvalidMaskException();
                    }

                    addMessageImp(buffer, written, i, format, args...);
                }
                else
                {
                    buffer[written++] = format[i];
                }
            }
        }

        void addIinitialMessage()
        {

        }

        void addFinalMessage()
        {

        }

    protected:

        ELogVerbosity mVerbosity;
        IFile& mFile;

    };

} // namespace Berserk

#endif //BERSERK_LOGMANAGER_H