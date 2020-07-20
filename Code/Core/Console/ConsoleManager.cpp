/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/ConsoleManager.h>

namespace Berserk {

    ConsoleManager* ConsoleManager::gConsoleManager = nullptr;

    ConsoleManager::ConsoleManager() {
        if (gConsoleManager) {
            fprintf(stderr, "[BERSERK Core] Only single Console Manager could be set as singleton");
            return;
        }

        gConsoleManager = this;
    }

    ConsoleManager& ConsoleManager::getSingleton() {
        return *gConsoleManager;
    }

    const char* ConsoleManager::getConsoleModificationModeString(EConsoleMod mod) {
        switch (mod) {
            case EConsoleMod::ByCode:
                return "ByCode";
            case EConsoleMod::ByUser:
                return "ByUser";
            default:
                return "Undefined";
        }
    }

}
