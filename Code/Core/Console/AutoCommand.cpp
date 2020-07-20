/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/AutoCommand.h>

namespace Berserk {

    AutoCommand::AutoCommand(const char *name, ConsoleCommand::Signature function, const char *help,
                             const TEnumMask<EConsoleFlag> &flags) {
        mCommand = ConsoleManager::getSingleton().registerCommand(name, std::move(function), help, flags).getPtr();
        BERSERK_COND_ERROR(mCommand, "Failed to register console command '%s'", name);
    }

    ConsoleCommand* AutoCommand::getObject() const {
        return mCommand;
    }

}