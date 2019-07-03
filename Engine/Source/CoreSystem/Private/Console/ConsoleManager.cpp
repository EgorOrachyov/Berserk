//
// Created by Egor Orachyov on 01.07.2019.
//

#include <Console/ConsoleManager.h>
#include "ConsoleCommand.h"
#include "ConsoleVariable.h"

namespace Berserk
{
    ConsoleManager::ConsoleManager(IAllocator &allocator, uint32 objectsCount, uint32 poolSize, uint32 historySize)
            : mAllocator(allocator),
              mMemoryPool(THashMap<String,IConsoleObject*>::getNodeSize(), poolSize, allocator),
              mObjects(objectsCount, allocator, mMemoryPool),
              mHistory(historySize, allocator)
    {
        mObjects.setHashFunction(String::hash);
    }

    ConsoleManager::~ConsoleManager()
    {
        for (auto pair = mObjects.begin(); pair != nullptr; pair = mObjects.next())
        {
            IConsoleObject* object = *pair->value();
            object->~IConsoleObject();
            mAllocator.free(object);
        }
    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, int32 initialValue, const char *help,
                                                       IConsoleVariable::OnChangeCallback callback,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        IConsoleObject* object = findObjectInternal(name);
        assertion_dev_msg(object == nullptr, "ConsoleManager: an attempt to re-register object [name: %s]", name);

        IConsoleVariable* variable = new (mAllocator.allocate(sizeof(ConsoleVariable<int32>)))
                ConsoleVariable<int32>(name, help, initialValue, callback, flags, priority);

        mObjects.put(variable->getName(), variable);
        return variable;
    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, float32 initialValue, const char *help,
                                                       IConsoleVariable::OnChangeCallback callback,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        IConsoleObject* object = findObjectInternal(name);
        assertion_dev_msg(object == nullptr, "ConsoleManager: an attempt to re-register object [name: %s]", name);

        IConsoleVariable* variable = new (mAllocator.allocate(sizeof(ConsoleVariable<float32>)))
                ConsoleVariable<float32>(name, help, initialValue, callback, flags, priority);

        mObjects.put(variable->getName(), variable);
        return variable;
    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, const char *initialValue, const char *help,
                                                       IConsoleVariable::OnChangeCallback callback,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        IConsoleObject* object = findObjectInternal(name);
        assertion_dev_msg(object == nullptr, "ConsoleManager: an attempt to re-register object [name: %s]", name);

        IConsoleVariable* variable = new (mAllocator.allocate(sizeof(ConsoleVariable<String>)))
                ConsoleVariable<String>(name, help, String(initialValue), callback, flags, priority);

        mObjects.put(variable->getName(), variable);
        return variable;
    }

    IConsoleCommand* ConsoleManager::registerCommand(const char *name, const char *help,
                                                     IConsoleCommand::ExecuteFunction function,
                                                     EConsoleObjectFlags flags, EConsolePriority priority)
    {
        CriticalSection section(mMutex);

        IConsoleObject* object = findObjectInternal(name);
        assertion_dev_msg(object == nullptr, "ConsoleManager: an attempt to re-register object [name: %s]", name);
        assertion_dev_msg(function != nullptr, "ConsoleManager: an attempt to re-register null command [name: %s]", name);

        IConsoleCommand* command = new (mAllocator.allocate(sizeof(IConsoleCommand)))
                ConsoleCommand(name, help, function, flags, priority);

        mObjects.put(command->getName(), command);
        return command;
    }

    IConsoleObject* ConsoleManager::findObject(const char *name)
    {
        CriticalSection section(mMutex);
        return findObjectInternal(String(name));
    }

    IConsoleVariable* ConsoleManager::findVariable(const char *name)
    {
        CriticalSection section(mMutex);
        IConsoleObject* object = findObjectInternal(String(name));
        return object->asVariable();
    }

    IConsoleCommand* ConsoleManager::findCommand(const char *name)
    {
        CriticalSection section(mMutex);
        IConsoleObject* object = findObjectInternal(String(name));
        return object->asCommand();
    }

    bool ConsoleManager::processInput(const char *input, IOutputDevice &device)
    {
        CriticalSection section(mMutex);

        TArray<String> args(mAllocator);
        String::split(input, " =,",  args);

        const uint32 argsCount = args.getSize();

        if (argsCount == 0)
        {
            device.printf("Empty arguments list\n");
            return false;
        }

        String& name = args.get(0);
        auto object = findObjectInternal(name);

        if (object == nullptr)
        {
            device.printf("No such command or variable [name: %s]\n", name.get());
            return false;
        }

        IConsoleVariable* var = object->asVariable();
        IConsoleCommand* cmd = object->asCommand();

        if (var != nullptr)
        {
            if (argsCount == 1)
            {
                device.printf("%s = %s ['%s']\n",
                              name.get(), var->getString().get(),
                              priorityToString(var->getPriority()));

                mHistory.add(String(input));
                return true;
            }

            if (argsCount == 2)
            {
                String& value = args.get(1);

                if (value == "?")
                {
                    device.printf("\n%s\n", var->getHelp().get());
                    mHistory.add(String(input));
                    return true;
                }

                String prevValue = var->getString();
                EConsolePriority prevPriority = var->getPriority();

                bool done = var->set(value.get(), EConsolePriority::SetByConsole);

                if (done)
                {
                    device.printf("%s = %s [Prev '%s' '%s']\n",
                                  name.get(), value.get(),
                                  prevValue.get(), priorityToString(prevPriority));

                    mHistory.add(String(input));
                }
                else
                {
                    device.printf("Cannot set [value: %s]\n", value.get());
                }
            }
            else
            {
                device.printf("Cannot set value: too much arguments\n");
                return false;
            }
        }
        else
        {
            if (argsCount == 2)
            {
                String& value = args.get(1);

                if (value == "?")
                {
                    device.printf("\n%s\n", cmd->getHelp().get());
                    mHistory.add(String(input));
                    return true;
                }
            }

            bool done = cmd->execute(args, device, EConsolePriority::SetByConsole);

            if (done)
            {
                mHistory.add(String(input));
            }
            else
            {
                device.printf("Command was not executed\n");
                return false;
            }
        }

        return true;
    }

    void ConsoleManager::getConsoleHistory(TArray<String> &out)
    {
        CriticalSection section(mMutex);
        out.append(mHistory);
    }

    void ConsoleManager::clearConsoleHistory()
    {
        CriticalSection section(mMutex);
        mHistory.clear();
    }

    IConsoleObject* ConsoleManager::findObjectInternal(const String &name)
    {
        IConsoleObject** object = mObjects.get(name);
        if (object != nullptr)
        {
            return *object;
        }

        return nullptr;
    }

    const char* ConsoleManager::priorityToString(EConsolePriority priority)
    {
        switch (priority)
        {
            case EConsolePriority::SetByCode:
                return "SetByCode";

            case EConsolePriority::SetByConfig:
                return "SetByConfig";

            case EConsolePriority::SetByConsole:
                return "SetByConsole";

            default:
                return "Unknown";
        }

    }

} // namespace Berserk