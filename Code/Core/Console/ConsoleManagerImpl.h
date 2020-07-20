/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLEMANAGERIMPL_H
#define BERSERK_CONSOLEMANAGERIMPL_H

#include <Console/ConsoleManager.h>
#include <AllocPool.h>
#include <Containers/TMap.h>

namespace Berserk {

    class ConsoleManagerImpl : public ConsoleManager {
    public:

        ConsoleManagerImpl();
        ~ConsoleManagerImpl();

        TRef<ConsoleVariable> registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;
        TRef<ConsoleVariable> registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;
        TRef<ConsoleVariable> registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;

        TRef<ConsoleVariable> registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;
        TRef<ConsoleVariable> registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;
        TRef<ConsoleVariable> registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;

        TRef<ConsoleCommand> registerCommand(const char *name, ConsoleCommand::Signature function, const char *help, const TEnumMask<EConsoleFlag> &flags) override;

        TRef<ConsoleVariable> findVariable(const CString &name) const override;

        void processUserInput(const CString &input, OutputDevice &outputDevice) override;
        void forEachConsoleObjectWithPrefix(const char *prefix, const Function<void(const ConsoleObject &)> &visitor) const override;

    private:

        TMap<CString, ConsoleObject*> mConsoleObjects;
        AllocPool mVariablesAllocator;
        AllocPool mCommandsAllocator;

        mutable Mutex mAccessMutex;

    };

}

#endif //BERSERK_CONSOLEMANAGERIMPL_H