/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_AUTOCOMMAND_H
#define BERSERK_AUTOCOMMAND_H

#include <Console/IConsoleManager.h>

namespace Berserk {

    class AutoCommand {
    public:
        AutoCommand() = default;
        AutoCommand(const char* name, IConsoleCommand::Signature function, const char* help, const TEnumMask<EConsoleFlag> &flags);
        IConsoleCommand* getObject() const;
    private:
        IConsoleCommand* mCommand = nullptr;
    };

}

#endif //BERSERK_AUTOCOMMAND_H