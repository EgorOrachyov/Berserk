/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/AutoVariable.h>

namespace Berserk {

    AutoConsoleVarInt::AutoConsoleVarInt(const char *name) {
        mVariable = IConsoleManager::getSingleton().findVariable(name).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to find variable '%s'", name);
    }

    AutoConsoleVarInt::AutoConsoleVarInt(const char *name, int32 defaultValue, const char *help,
                                         const TEnumMask<EConsoleFlag> &flags) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    AutoConsoleVarInt::AutoConsoleVarInt(const char *name, int32 defaultValue, const char *help,
                                         const TEnumMask<EConsoleFlag> &flags,
                                         Mutex &access) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags, access).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    int32 AutoConsoleVarInt::get() const {
        return (mVariable ? mVariable->getInt() : 0);
    }

    IConsoleVariable *AutoConsoleVarInt::getObject() const {
        return mVariable;
    }


    AutoConsoleVarFloat::AutoConsoleVarFloat(const char *name) {
        mVariable = IConsoleManager::getSingleton().findVariable(name).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to find variable '%s'", name);
    }

    AutoConsoleVarFloat::AutoConsoleVarFloat(const char *name, float defaultValue, const char *help,
                                             const TEnumMask<EConsoleFlag> &flags) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    AutoConsoleVarFloat::AutoConsoleVarFloat(const char *name, float defaultValue, const char *help,
                                             const TEnumMask<EConsoleFlag> &flags,
                                             Mutex &access) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags, access).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    float AutoConsoleVarFloat::get() const {
        return (mVariable ? mVariable->getFloat() : 0.0f);
    }

    IConsoleVariable *AutoConsoleVarFloat::getObject() const {
        return mVariable;
    }


    AutoConsoleVarString::AutoConsoleVarString(const char *name) {
        mVariable = IConsoleManager::getSingleton().findVariable(name).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to find variable '%s'", name);
    }

    AutoConsoleVarString::AutoConsoleVarString(const char *name, const char *defaultValue, const char *help,
                                               const TEnumMask<EConsoleFlag> &flags) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    AutoConsoleVarString::AutoConsoleVarString(const char *name, const char *defaultValue, const char *help,
                                               const TEnumMask<EConsoleFlag> &flags,
                                               Mutex &access) {
        mVariable = IConsoleManager::getSingleton().registerVariable(name, defaultValue, help, flags, access).getPtr();
        BERSERK_COND_ERROR(mVariable, "Failed to register variable '%s'", name);
    }

    CString AutoConsoleVarString::get() const {
        return (mVariable ? mVariable->getString() : CString());
    }

    IConsoleVariable* AutoConsoleVarString::getObject() const {
        return mVariable;
    }

}