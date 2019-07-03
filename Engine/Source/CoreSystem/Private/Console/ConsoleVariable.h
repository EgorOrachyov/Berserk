//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_CONSOLEVARIABLE_H
#define BERSERK_CONSOLEVARIABLE_H

#include <Console/IConsoleVariable.h>
#include <Threading/Mutex.h>

namespace Berserk
{

    /**
     * Console variable internal implementation for console manager
     * @tparam TypeValue Type of the variable value
     */
    template <typename TypeValue>
    class ConsoleVariable : public IConsoleVariable
    {
    public:

        GENERATE_NEW_DELETE(ConsoleVariable);

        ConsoleVariable(Mutex& mutex,
                        const char* name, const char* help,
                        const TypeValue& initial, OnChangeCallback callback = nullptr,
                        EConsoleObjectFlags flags = EConsoleObjectFlags::Default,
                        EConsolePriority priority = EConsolePriority::SetByCode)
                : mName(name),
                  mHelpText(help),
                  mValue(initial),
                  mObjectFlags(flags),
                  mPriority(priority),
                  mCallbackFunction(callback),
                  mMutex(mutex)

        {

        }

        ~ConsoleVariable() override = default;

        bool set(const char *value, EConsolePriority priority) override;

        void setOnChangeCallback(OnChangeCallback function) override { mCallbackFunction = function; }

        int32 getInt() const override;

        float32 getFloat() const override;

        String getString() const override;

        const String &getName() const override { return mName; }

        const String &getHelp() const override { return mHelpText; }

        EConsoleObjectFlags getFlags() const override { return mObjectFlags; }

        EConsolePriority getPriority() const override { return mPriority; }

        IConsoleCommand *asCommand() const override { return nullptr; }

        IConsoleVariable *asVariable() const override { return (IConsoleVariable*) this; }

        bool canSet(EConsolePriority priority) const { return mPriority <= priority; }

        void callOnChange() const
        {
            if (mCallbackFunction != nullptr)
            {
                IConsoleVariable* variable = (IConsoleVariable*) this;
                mCallbackFunction(variable);
            }
        }

    private:

        /** Console manager modification mutex for this var (read/write) */
        Mutex &mMutex;

        OnChangeCallback mCallbackFunction;
        EConsolePriority mPriority;
        EConsoleObjectFlags mObjectFlags;

        String mName;
        String mHelpText;

        TypeValue mValue;


    };

    template <>
    bool ConsoleVariable<int32>::set(const char *value, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        if (canSet(priority))
        {
            mValue = String::toInt32(value);
            callOnChange();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <>
    int32 ConsoleVariable<int32>::getInt() const
    {
        CriticalSection section(mMutex);
        return mValue;
    }

    template <>
    float32 ConsoleVariable<int32>::getFloat() const
    {
        CriticalSection section(mMutex);
        return (float32) mValue;
    }

    template <>
    String ConsoleVariable<int32>::getString() const
    {
        CriticalSection section(mMutex);
        return String::toString(mValue);
    }

    template <>
    bool ConsoleVariable<float32>::set(const char *value, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        if (canSet(priority))
        {
            mValue = String::toFloat32(value);
            callOnChange();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <>
    int32 ConsoleVariable<float32>::getInt() const
    {
        CriticalSection section(mMutex);
        return (int32) mValue;
    }

    template <>
    float32 ConsoleVariable<float32>::getFloat() const
    {
        CriticalSection section(mMutex);
        return mValue;
    }

    template <>
    String ConsoleVariable<float32>::getString() const
    {
        CriticalSection section(mMutex);
        return String::toString(mValue);
    }

    template <>
    bool ConsoleVariable<String>::set(const char *value, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        if (canSet(priority))
        {
            mValue = value;
            callOnChange();
            return true;
        }
        else
        {
            return false;
        }
    }

    template <>
    int32 ConsoleVariable<String>::getInt() const
    {
        CriticalSection section(mMutex);
        return String::toInt32(mValue.get());
    }

    template <>
    float32 ConsoleVariable<String>::getFloat() const
    {
        CriticalSection section(mMutex);
        return String::toFloat32(mValue.get());
    }

    template <>
    String ConsoleVariable<String>::getString() const
    {
        CriticalSection section(mMutex);
        String result(mValue);
        return result;
    }

} // namespace Berserk

#endif //BERSERK_CONSOLEVARIABLE_H
