//
// Created by Egor Orachyov on 14.06.2019.
//

#ifndef BERSERK_LOGGINGTEST_H
#define BERSERK_LOGGINGTEST_H

#include <IO/PlatformFileDev.h>
#include <IO/CachedFileWriter.h>
#include <Logging/ILogManager.h>
#include <Logging/LogManager.h>
#include <Logging/Debug.h>
#include <Logging/DebugLogMacros.h>

using namespace Berserk;

class LoggingTest
{
public:

    static void LogManagerTest1()
    {
        PlatformFileDev fileDev("../Test/Files/log.txt", false, true);
        CachedFileWriter file(fileDev, KiB);
        LogManager logManager(file);

        logManager.addMessagef(Error, false, "It is %i or %5.4f", 12, 0.43325);
        logManager.addMessage("Some category", "Some message", ELogVerbosity::Fatal, true);
        logManager.addMessage("Other message", ELogVerbosity::Display, true);
        logManager.addPage();

        logManager.addMessagef(Warning, false, "Something %f", 0.333333);
        logManager.addMessage("Some category", "Some message", ELogVerbosity::Fatal, true);
        logManager.addMessage("Other message", ELogVerbosity::Display, true);
        logManager.addPage();
    }

    static void LogManagerTest2()
    {
        PlatformFileDev fileDev("../Test/Files/log.txt", false, true);
        CachedFileWriter file(fileDev, KiB);
        LogManager logManager(file);

        try {
            logManager.addMessagef((ELogVerbosity)11, false, "It is %i or %5.4f", 12, 0.43325);
        } catch (Exception& e) {
            e.out();
        }

        try {
            logManager.addMessagef(Fatal, false, "It is %i or %5.4", 12, 0.43325);
        } catch (Exception& e) {
            e.out();
        }

        try {
            logManager.addMessagef(Fatal, false, "String %s", "some string");
        } catch (Exception& e) {
            e.out();
        }
    }

    static void LogManagerTest3()
    {
        Debug::get().addMessagef(ELogVerbosity::Warning, false, "%s %s", "test", "debug log");
    }

    static void LogManagerTest4()
    {
        LOG(ELogVerbosity::Warning, "%s %s", "test", "debug log");
    }

    static void run()
    {
        LogManagerTest1();
        LogManagerTest2();
        LogManagerTest3();
        LogManagerTest4();
    }

};


#endif //BERSERK_LOGGINGTEST_H