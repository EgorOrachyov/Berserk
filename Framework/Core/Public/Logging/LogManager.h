//
// Created by Egor Orachyov on 24.01.2019.
//

#ifndef BERSERK_LOGMANAGER_H
#define BERSERK_LOGMANAGER_H

#include "Public/Misc/Types.h"
#include "Public/Misc/Include.h"
#include "Public/Misc/UsageDescriptors.h"
#include "Public/Logging/LogVerbosity.h"

namespace Berserk
{

    class IO_API LogManager {

    public:

        explicit LogManager(const char* logFileName);
        ~LogManager();

        void addMessage(LogVerbosity verbosity, const char* message);
        //...

        static LogManager& getSingleton();

    private:

        void flush();
        void beginLog();
        void endLog();

    private:

        FILE*  mLogFile;
        uint32 mLinesCount;
        uint32 mMessagesCount;


    };

}

#endif //BERSERK_LOGMANAGER_H
