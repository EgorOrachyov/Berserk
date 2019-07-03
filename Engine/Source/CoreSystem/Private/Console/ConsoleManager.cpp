//
// Created by Egor Orachyov on 01.07.2019.
//

#include <Console/ConsoleManager.h>
#include <Console/ConsoleCommand.h>
#include <Console/ConsoleVariable.h>

namespace Berserk
{
    ConsoleManager::ConsoleManager(IAllocator &allocator, uint32 objectsCount, uint32 poolSize)
            : mAllocator(allocator),
              mMemoryPool(THashMap<String,IConsoleObject*>::getNodeSize(), poolSize, allocator),
              mObjects(objectsCount, allocator, mMemoryPool),
              mHistory(allocator)
    {

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
        return findObjectInternal(name);
    }

    IConsoleVariable* ConsoleManager::findVariable(const char *name)
    {
        CriticalSection section(mMutex);
        IConsoleObject* object = findObjectInternal(name);
        return object->asVariable();
    }

    IConsoleCommand* ConsoleManager::findCommand(const char *name)
    {
        CriticalSection section(mMutex);
        IConsoleObject* object = findObjectInternal(name);
        return object->asCommand();
    }

    bool ConsoleManager::processInput(const char *input, IOutputDevice &device)
    {
        CriticalSection section(mMutex);
        return false;
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

    IConsoleObject* ConsoleManager::findObjectInternal(const char *name)
    {
        IConsoleObject** object = mObjects.get(String(name));
        if (object != nullptr)
        {
            return *object;
        }

        return nullptr;
    }

} // namespace Berserk