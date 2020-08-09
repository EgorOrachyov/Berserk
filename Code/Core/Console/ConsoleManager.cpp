/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Console/ConsoleManager.h>
#include <ErrorMacro.h>
#include <LogMacro.h>
#include <Variant.h>

namespace Berserk {

    class ConsoleVariableImpl : public ConsoleVariable {
    public:

        ConsoleVariableImpl(int32 value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
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

        ConsoleVariableImpl(float value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
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

        ConsoleVariableImpl(CString value, EConsoleMod mod, CString name, CString help, const TEnumMask<EConsoleFlag> flags, TRef<Mutex> mutex) {
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

        ~ConsoleVariableImpl() override = default;

        const TEnumMask<EConsoleFlag> &getFlags() const override { return mFlags; }
        const CString &getName() const override { return mName; }
        const CString &getHelpText() const override { return mHelpText; }
        bool isVariable() const override { return true; }
        bool isCommand() const override { return false; }

        bool isInt() const override { return mType == EVariantType::Int; }
        bool isFloat() const override { return mType == EVariantType::Float; }
        bool isString() const override { return mType == EVariantType::String; }

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

        bool set(const CString &value, EConsoleMod mod) override {
            bool ableToChange;

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);
                ableToChange = canChange(mod);
            }
            else {
                ableToChange = canChange(mod);
            }

            if (!ableToChange) {
                BERSERK_LOG_INFO("Failed to set console variable '%s' with EConsoleMod::%s (Last set with EConsoleMod::%s)",
                                 mName.data(),
                                 ConsoleManager::getConsoleModAsString(mod),
                                 ConsoleManager::getConsoleModAsString(mModification));
                return false;
            }

            if (mMutex.isNotNull()) {
                Guard guard(*mMutex);
                mModification = mod;

                switch (mType) {
                    case EVariantType::Int: {
                        mValueInt = value.toInt32();
                        mValueFloat = mValueInt;
                        mValueStr = value;
                        return true;
                    }
                    case EVariantType::Float: {
                        mValueFloat = value.toFloat();
                        mValueInt = (int32) mValueFloat;
                        mValueStr = value;
                        return true;
                    }
                    case EVariantType::String: {
                        mValueStr = value;
                        mValueInt = 0;
                        mValueFloat = 0.0f;
                        return true;
                    }
                    default:
                        return false;
                }
            } else {
                mModification = mod;

                switch (mType) {
                    case EVariantType::Int: {
                        mValueInt = value.toInt32();
                        mValueFloat = mValueInt;
                        mValueStr = value;
                        return true;
                    }
                    case EVariantType::Float: {
                        mValueFloat = value.toFloat();
                        mValueInt = (int32) mValueFloat;
                        mValueStr = value;
                        return true;
                    }
                    case EVariantType::String: {
                        mValueStr = value;
                        mValueInt = 0;
                        mValueFloat = 0.0f;
                        return true;
                    }
                    default:
                        return false;
                }
            }
        }

    private:

        bool canChange(EConsoleMod mod) const {
            return (uint32)mod >= (uint32)mModification || mFlags.getFlag(EConsoleFlag::IgnorePriority);
        }

        EConsoleMod mModification = EConsoleMod::ByConstructor;
        EVariantType mType;
        int32 mValueInt;
        float mValueFloat;
        CString mValueStr;
        CString mHelpText;
        CString mName;
        TEnumMask<EConsoleFlag> mFlags;
        mutable TRef<Mutex> mMutex;
    };



    class ConsoleCommandImpl : public ConsoleCommand {
    public:

        ConsoleCommandImpl(CString name, CString help, Signature& body, const TEnumMask<EConsoleFlag> &flags) {
            mName = std::move(name);
            mHelpText = std::move(help);
            mFunction = std::move(body);
            mFlags = flags;
        }
        ~ConsoleCommandImpl() override = default;

        const TEnumMask<EConsoleFlag> &getFlags() const override { return mFlags; }
        const CString &getName() const override { return mName; }
        const CString &getHelpText() const override { return mHelpText; }

        bool isVariable() const override { return false; }
        bool isCommand() const override { return true; }

        void execute(const TArray<CString> &args, OutputDevice &device) override { mFunction(args, device); }

    private:

        CString mName;
        CString mHelpText;
        TEnumMask<EConsoleFlag> mFlags;
        Signature mFunction;

    };

    ConsoleManager* ConsoleManager::gConsoleManager = nullptr;

    ConsoleManager::ConsoleManager()
        : mVariablesAllocator(sizeof(ConsoleVariableImpl)),
          mCommandsAllocator(sizeof(ConsoleCommandImpl)) {

        BERSERK_COND_ERROR_RET(gConsoleManager == nullptr, "Only single ConsoleManager could be set as singleton");
        gConsoleManager = this;

        BERSERK_LOG_INFO("Initialize ConsoleManager");
    }

    ConsoleManager::~ConsoleManager() {
        for (auto& pair: mConsoleObjects) {
            pair.second()->~ConsoleObject();
            pair.second() = nullptr;
        }

        BERSERK_LOG_INFO("Finalize ConsoleManager");
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, variableName, help, flags, nullptr);
        mConsoleObjects.emplace(variableName, variable);

        return variable;
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, name, help, flags, nullptr);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, name, help, flags, nullptr);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, int32 defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, float defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<ConsoleVariable> ConsoleManager::registerVariable(const char *name, const char *defaultValue, const char *help, const TEnumMask<EConsoleFlag> &flags, Mutex &access) {
        CString variableName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(variableName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mVariablesAllocator.allocate(sizeof(ConsoleVariableImpl));
        ConsoleVariableImpl* variable = new (memory) ConsoleVariableImpl(defaultValue, EConsoleMod::ByConstructor, name, help, flags, access);
        mConsoleObjects.emplace(name, variable);

        return variable;
    }

    TRef<ConsoleCommand> ConsoleManager::registerCommand(const char *name, ConsoleCommand::Signature function, const char *help, const TEnumMask<EConsoleFlag> &flags) {
        CString commandName = name;

        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(commandName);

        if (found.isNotNull())
            return nullptr;

        void* memory = mCommandsAllocator.allocate(sizeof(ConsoleCommandImpl));
        ConsoleCommandImpl* command = new (memory) ConsoleCommandImpl(commandName, help, function, flags);
        mConsoleObjects.emplace(commandName, command);

        return command;
    }

    TRef<ConsoleVariable> ConsoleManager::findVariable(const CString &name) const {
        Guard guard(mAccessMutex);
        auto found = mConsoleObjects.getPtr(name);

        if (found.isNotNull() && (*found)->isVariable())
            return (ConsoleVariable*)*found;

        return nullptr;
    }

    void ConsoleManager::processUserInput(const CString &input, OutputDevice &outputDevice) {
        mParsingResults.clear();
        TArray<CString> &result = mParsingResults;

        CString::split(input, " ", result);

        if (result.size() == 0)
            return;

        {
            Guard guard(mAccessMutex);
            auto found = mConsoleObjects.getPtr(result[0]);

            if (found.isNotNull()) {
                if ((*found)->isVariable()) {
                    auto variable = (ConsoleVariable*)*found;

                    // Show only value info
                    if (result.size() == 1) {
                        outputDevice.printf(EOutputType::Text, "%s %s (Last set with EConsoleMod::%s)",
                                variable->getName().data(),
                                variable->getString().data(),
                                getConsoleModAsString(variable->getModificationType()));
                        return;
                    }

                    // If args != 2 -> user doing something wrong
                    if (result.size() != 2) {
                        outputDevice.print(EOutputType::Text, "Enter 'var ?' to get info about variable");
                        outputDevice.print(EOutputType::Text, "Enter 'var value' to set new variable value");
                        return;
                    }

                    // Show info
                    if (result[1] == "?") {
                        outputDevice.print(EOutputType::Text, variable->getHelpText().data());
                        return;
                    }

                    // Set new value
                    {
                        auto wasModified = variable->set(result[1], EConsoleMod::ByUser);

                        if (wasModified) {
                            outputDevice.printf(EOutputType::Text, "Set variable %s %s", variable->getName().data(), variable->getString().data());
                        }
                    }

                }
                else {
                    auto command = (ConsoleCommand*)*found;
                    command->execute(result, outputDevice);
                }
            }
        }
    }

    void ConsoleManager::forEachConsoleObjectWithPrefix(const char *prefix, const Function<void(const ConsoleObject &)> &visitor) const {
        Guard guard(mAccessMutex);
        for (const auto& pair: mConsoleObjects) {
            if (pair.first().starts(prefix))
                visitor(*pair.second());
        }
    }

    ConsoleManager& ConsoleManager::getSingleton() {
        return *gConsoleManager;
    }

    const char* ConsoleManager::getConsoleModAsString(EConsoleMod mod) {
        switch (mod) {
            case EConsoleMod::ByConstructor:
                return "ByConstructor";
            case EConsoleMod::ByConfig:
                return "ByConfig";
            case EConsoleMod::ByCode:
                return "ByCode";
            case EConsoleMod::ByUser:
                return "ByUser";
            default:
                return "Undefined";
        }
    }

}
