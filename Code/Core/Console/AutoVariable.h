/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_AUTOVARIABLE_H
#define BERSERK_AUTOVARIABLE_H

#include <Console/IConsoleManager.h>

namespace Berserk {

    class AutoConsoleVarInt {
    public:
        AutoConsoleVarInt() = default;
        explicit AutoConsoleVarInt(const char* name);
        AutoConsoleVarInt(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);
        AutoConsoleVarInt(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);
        int32 get() const;
        IConsoleVariable* getObject() const;
    private:
        IConsoleVariable* mVariable = nullptr;
    };

    class AutoConsoleVarFloat {
    public:
        AutoConsoleVarFloat() = default;
        explicit AutoConsoleVarFloat(const char* name);
        AutoConsoleVarFloat(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);
        AutoConsoleVarFloat(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);
        float get() const;
        IConsoleVariable* getObject() const;
    private:
        IConsoleVariable* mVariable = nullptr;
    };

    class AutoConsoleVarString {
    public:
        AutoConsoleVarString() = default;
        explicit AutoConsoleVarString(const char* name);
        AutoConsoleVarString(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);
        AutoConsoleVarString(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);
        CString get() const;
        IConsoleVariable* getObject() const;
    private:
        IConsoleVariable* mVariable = nullptr;
    };

}

#endif //BERSERK_AUTOVARIABLE_H
