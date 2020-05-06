/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/AutoCommand.h>

namespace Berserk {

    AutoCommand::AutoCommand(const char *name, Berserk::IConsoleCommand::Signature function, const char *help,
                             const Berserk::TEnumMask<Berserk::EConsoleFlag> &flags) {
        mCommand = IConsoleManager::getSingleton().registerCommand(name, std::move(function), help, flags).getPtr();
        BERSERK_COND_ERROR(mCommand, "Failed to register console command '%s'", name);
    }

    IConsoleCommand* AutoCommand::getObject() const {
        return mCommand;
    }

}