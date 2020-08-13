/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLECOMMANDS_H
#define BERSERK_CONSOLECOMMANDS_H

#include <Console/AutoCommand.h>

namespace Berserk {

    /** Debug commands exposed to the in-game console */
    class ConsoleCommands {
    public:
        ConsoleCommands();
        ~ConsoleCommands();

    private:
        AutoCommand mDebugStat;
        AutoCommand mDebugLogInfo;
        AutoCommand mDebugLogWarning;
        AutoCommand mDebugLogError;
        AutoCommand mDebugClearBatch;
        AutoCommand mDebugDrawBox;
        AutoCommand mDebugDrawSphere;
        AutoCommand mDebugDrawCylinder;
    };

}

#endif //BERSERK_CONSOLECOMMANDS_H