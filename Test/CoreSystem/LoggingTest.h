//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGGINGTEST_H
#define BERSERK_LOGGINGTEST_H

#include <IO/PlatformFileDev.h>
#include <IO/CachedFileWriter.h>
#include <Logging/LogManager.h>

using namespace Berserk;

class LoggingTest
{
public:

    static void LogManagerTest()
    {
        PlatformFileDev fileDev("../Test/Files/log.txt");
        CachedFileWriter file(fileDev, KiB);
        LogManager logManager(file);

        logManager.addMessage(Error, false, "It is %i or %5 ", 12, 0.43325);
    }

    static void run()
    {
        LogManagerTest();
    }

};


#endif //BERSERK_LOGGINGTEST_H
