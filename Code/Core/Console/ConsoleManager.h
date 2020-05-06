/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLEMANAGER_H
#define BERSERK_CONSOLEMANAGER_H

#include <Console/IConsoleManager.h>
#include <AllocPool.h>
#include <TMap.h>

namespace Berserk {

    class ConsoleManager : public IConsoleManager {
    public:

        ConsoleManager();
        ~ConsoleManager();

        TRef<IConsoleVariable> registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;
        TRef<IConsoleVariable> registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;
        TRef<IConsoleVariable> registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) override;

        TRef<IConsoleVariable> registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;
        TRef<IConsoleVariable> registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;
        TRef<IConsoleVariable> registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) override;

        TRef<IConsoleCommand> registerCommand(const char *name, IConsoleCommand::Signature function, const char *help, const TEnumMask<EConsoleFlag> &flags) override;

        TRef<IConsoleVariable> findVariable(const CString &name) const override;

        void processUserInput(const CString &input, IOutputDevice &outputDevice) override;
        void forEachConsoleObjectWithPrefix(const char *prefix, const Function<void(const IConsoleObject &)> &visitor) const override;

    private:

        TMap<CString, IConsoleObject*> mConsoleObjects;
        AllocPool mVariablesAllocator;
        AllocPool mCommandsAllocator;

        mutable Mutex mAccessMutex;

    };

}

#endif //BERSERK_CONSOLEMANAGER_H