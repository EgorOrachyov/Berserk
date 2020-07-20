/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_AUTOCOMMAND_H
#define BERSERK_AUTOCOMMAND_H

#include <Console/ConsoleManager.h>

namespace Berserk {

    class AutoCommand {
    public:
        AutoCommand() = default;
        AutoCommand(const char* name, ConsoleCommand::Signature function, const char* help, const TEnumMask<EConsoleFlag> &flags);
        ConsoleCommand* getObject() const;
    private:
        ConsoleCommand* mCommand = nullptr;
    };

}

#endif //BERSERK_AUTOCOMMAND_H