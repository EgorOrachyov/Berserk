//
// Created by Egor Orachyov on 01.07.2019.
//

#include <Console/ConsoleManager.h>

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

    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, int32 initialValue, const char *help,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        return nullptr;
    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, float32 initialValue, const char *help,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        return nullptr;
    }

    IConsoleVariable* ConsoleManager::registerVariable(const char *name, const char *initialValue, const char *help,
                                                       EConsoleObjectFlags flags, EConsolePriority priority)
    {
        return nullptr;
    }

    IConsoleCommand* ConsoleManager::registerCommand(const char *name, const char *help, IConsoleCommand::ExecuteFunction function,
                                                     EConsoleObjectFlags flags, EConsolePriority priority)
    {
        return nullptr;
    }

    IConsoleObject* ConsoleManager::findObject(const char *name)
    {
        return nullptr;
    }

    IConsoleVariable* ConsoleManager::findVariable(const char *name)
    {
        return nullptr;
    }

    IConsoleCommand* ConsoleManager::findCommand(const char *name)
    {
        return nullptr;
    }

    bool ConsoleManager::processInput(const char *input, IOutputDevice &device)
    {
        return false;
    }

    void ConsoleManager::getConsoleHistory(TArray<String> &out)
    {

    }

    void ConsoleManager::clearConsoleHistory()
    {

    }
} // namespace Berserk