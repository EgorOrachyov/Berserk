//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include "Misc/Types.h"
#include "Misc/Include.h"
#include "Misc/UsageDescriptors.h"
#include "Logging/LogVerbosity.h"
#include "Strings/StringStream.h"

namespace Berserk
{

    class IO_API LogManager
    {
    public:

        typedef Strings<char,'\0'> Utils;

        /** Internal accumulator buffer for message before flushing in file */
        static const uint32 LOG_BUFFER_SIZE = Buffers::KiB * 10;

        /** Message sizes are limited */
        static const uint32 MAX_MESSAGE_SIZE = Buffers::KiB;

        /** Width of the log file (length for one line) */
        static const uint32 LINE_LENGTH = 100;

    public:

        /**
         * Creates log manager (will rewrite file with name logFileName)
         * @param filename Name for log file
         */
        explicit LogManager(const char *filename);

        ~LogManager();

        /** Add message in the log */
        void addMessage(LogVerbosity type, const char *message);

        /** Push message from the new empty line*/
        void push(LogVerbosity type, const char *message);

        /** Add line separator */
        void addLine();

        /** Add empty line separator */
        void addEmptyLine();

        /** Add head to split log content on parts */
        void addHeader(const char *title);

        /** Open block with chosen title in the log */
        void blockOpen(const char *title);

        /** Add message inside the block */
        void blockPush(const char *message);

        /** Close block with chosen title */
        void blockClose(const char *title);

        /** Set log verbosty (default = Display) */
        void setVerbosity(LogVerbosity verbosity);

        /** Enable or disable writing to the file */
        void setFileWriting(bool write);

        /** For fatal shut down of the engine */
        void explicitClose();

        /** Default log manager (used in the most engine subsystems) */
        static LogManager& getSingleton();

    private:

        void flush(const char *buffer);
        void beginLog();
        void endLog();

    private:

        FILE*  mLogFile;
        uint32 mLinesCount;
        uint32 mMessagesCount;
        uint32 mStreamLength;
        StringStream<char,'\0', LOG_BUFFER_SIZE> mStream;
        LogVerbosity mLogVerbosity;

        bool mWriteToFile;

    };

}

#endif //BERSERK_LOGMANAGER_H
