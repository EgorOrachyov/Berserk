/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/ConsoleManager.h>
#include <Reflection/Variant.h>
#include <String/String.h>
#include <LogMacro.h>

namespace Berserk {

    class ConsoleVariable : public IConsoleVariable {
    public:

        ConsoleVariable(int32 value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
            mName = std::move(name);
            mHelpText = std::move(help);
            mFlags = flags;
            mMutex = mutex;
            mValueInt = value;
            mValueFloat = value;
            mValueStr = CString::fromInt32(value);
            mType = EVariantType::Int;
            mModification = mod;
        }

        ConsoleVariable(float value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
            mName = std::move(name);
            mHelpText = std::move(help);
            mFlags = flags;
            mMutex = mutex;
            mValueFloat = value;
            mValueInt = (int32) value;
            mValueStr = CString::fromFloat(value);
            mType = EVariantType::Float;
            mModification = mod;
        }

        ConsoleVariable(CString value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
            mName = std::move(name);
            mHelpText = std::move(help);
            mFlags = flags;
            mMutex = mutex;
            mValueFloat = 0.0f;
            mValueInt = 0;
            mValueStr = std::move(value);
            mType = EVariantType::String;
            mModification = mod;
        }

        ~ConsoleVariable() override = default;

        const TEnumMask<EConsoleFlag> &getFlags() const override { return mFlags; }
        const CString &getName() const override { return mName; }
        const CString &getHelpText() const override { return mHelpText; }
        bool isVariable() const override { return true; }
        bool isCommand() const override { return false; }

        bool isInt() const override { return mType == EVariantType::Int; }
        bool isFloat() const override { return mType == EVariantType::Float; }
        bool isString() const override { return mType == EVariantType::String; }

        bool canChange(EConsoleMod mod) const override { return (uint32)mod >= (uint32)mModification || mFlags.getFlag(EConsoleFlag::IgnorePriority); }
        EConsoleMod getModificationType() const override { return mModification; }

        int32 getInt() const override {
            int32 result;

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);
                result = mValueInt;
            }
            else {
                result = mValueInt;
            }

            return result;
        }

        float getFloat() const override {
            float result;

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);
                result = mValueFloat;
            }
            else {
                result = mValueFloat;
            }

            return result;
        }

        CString getString() const override {
            CString result;

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);
                result = mValueStr;
            }
            else {
                result = mValueStr;
            }

            return result;
        }

    protected:

        void set(const CString &value, EConsoleMod mod) override {
            if (!canChange(mod)) {
                BERSERK_LOG_INFO("Failed to set console variable '%s' with EConsoleMod::%s (Last set with EConsoleMod::%s)",
                                 mName.data(),
                                 IConsoleManager::getConsoleModificationModeString(mod),
                                 IConsoleManager::getConsoleModificationModeString(mModification));
                return;
            }

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);

                switch (mType) {
                    case EVariantType::Int: {
                        mValueInt = value.toInt32();
                        mValueFloat = mValueInt;
                        mValueStr = value;
                        return;
                    }
                    case EVariantType::Float: {
                        mValueFloat = value.toFloat();
                        mValueInt = (int32) mValueFloat;
                        mValueStr = value;
                        return;
                    }
                    case EVariantType::String: {
                        mValueStr = value;
                        mValueInt = 0;
                        mValueFloat = 0.0f;
                        return;
                    }
                    default:
                        return;
                }
            } else {
                switch (mType) {
                    case EVariantType::Int: {
                        mValueInt = value.toInt32();
                        mValueFloat = mValueInt;
                        mValueStr = value;
                        return;
                    }
                    case EVariantType::Float: {
                        mValueFloat = value.toFloat();
                        mValueInt = (int32) mValueFloat;
                        mValueStr = value;
                        return;
                    }
                    case EVariantType::String: {
                        mValueStr = value;
                        mValueInt = 0;
                        mValueFloat = 0.0f;
                        return;
                    }
                    default:
                        return;
                }
            }
        }

        EConsoleMod mModification = EConsoleMod::ByCode;
        EVariantType mType;
        int32 mValueInt;
        float mValueFloat;
        CString mValueStr;
        CString mHelpText;
        CString mName;
        TEnumMask<EConsoleFlag> mFlags;

        mutable TRef<Mutex> mMutex;
    };

    class ConsoleCommand : public IConsoleCommand {
    public:

        ConsoleCommand(CString name, CString help, Signature& body, const TEnumMask<EConsoleFlag> &flags) {
            mName = std::move(name);
            mHelpText = std::move(help);
            mFunction = std::move(body);
            mFlags = flags;
        }
        ~ConsoleCommand() override = default;

        const TEnumMask<EConsoleFlag> &getFlags() const override { return mFlags; }
        const CString &getName() const override { return mName; }
        const CString &getHelpText() const override { return mHelpText; }

        bool isVariable() const override { return false; }
        bool isCommand() const override { return true; }

        void execute(const TArray<CString> &args, IOutputDevice &device) override { mFunction(args, device); }

    private:

        CString mHelpText;
        CString mName;
        TEnumMask<EConsoleFlag> mFlags;
        Signature mFunction;

    };

    ConsoleManager::ConsoleManager()
        : IConsoleManager(),
          mVariablesAllocator(sizeof(ConsoleVariable)),
          mCommandsAllocator(sizeof(ConsoleCommand)) {
        BERSERK_LOG_INFO("Initialize ConsoleManager");
    }

    ConsoleManager::~ConsoleManager() {
        for (auto& pair: mConsoleObjects) {
            pair.second()->~IConsoleObject();
            pair.second() = nullptr;
        }
        BERSERK_LOG_INFO("Finalize ConsoleManager");
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, variableName, help, flags, nullptr);
        mConsoleObjects.emplace(variableName, variable);

        return variable;
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, name, help, flags, nullptr);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, name, help, flags, nullptr);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<IConsoleVariable> ConsoleManager::registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariable));
        ConsoleVariable* variable = new (memory) ConsoleVariable(defaultValue, EConsoleMod::ByCode, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<IConsoleCommand> ConsoleManager::registerCommand(const char *name, IConsoleCommand::Signature function, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString commandName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(commandName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mCommandsAllocator.allocate(sizeof(ConsoleCommand));
        ConsoleCommand* command = new (memory) ConsoleCommand(commandName, help, function, flags);
        mConsoleObjects.emplace(commandName, command);

        return command;
    }

    TRef<IConsoleVariable> ConsoleManager::findVariable(const CString &name) const {
        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(name);

        if (found.isNotNull() && (*found)->isVariable())
            return (IConsoleVariable*)*found;

        return nullptr;
    }

    void ConsoleManager::processUserInput(const CString &input, IOutputDevice &outputDevice) {
        TArray<CString> result;
        String::split(input, " ", result);

        if (result.size() == 0)
            return;

        {
            Guard guard(mAccessMutex);
            auto found = mConsoleObjects.getPtr(result[0]);

            if (found.isNotNull()) {
                if ((*found)->isVariable()) {
                    auto variable = (IConsoleVariable*)*found;

                    if (result.size() != 2)
                        return;

                    if (result[1] == "?") {
                        BERSERK_LOG_INFO("%s", variable->getHelpText().data());
                        return;
                    }

                    variable->set(result[1], EConsoleMod::ByUser);
                }
                else {
                    auto command = (IConsoleCommand*)*found;
                    command->execute(result, outputDevice);
                }
            }
        }
    }

    void ConsoleManager::forEachConsoleObjectWithPrefix(const char *prefix, const Function<void(const IConsoleObject &)> &visitor) const {
        Guard guard(mAccessMutex);
        for (const auto& pair: mConsoleObjects) {
            if (pair.first().starts(prefix))
                visitor(*pair.second());
        }
    }

}