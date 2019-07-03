//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_CONSOLEMANAGER_H
#define BERSERK_CONSOLEMANAGER_H

#include <Console/IConsoleManager.h>
#include <Containers/THashMap.h>

namespace Berserk
{

    /**
     * Default engine console manager implementation
     * @note Thread-Safe
     */
    class CORE_API ConsoleManager : public IConsoleManager
    {
    public:

        GENERATE_NEW_DELETE(ConsoleManager);

        /**
         * Initialize console manager an preallocate need memory for internal usage.
         * @param allocator Engine allocator to allocate resources for this manager
         * @param objectsCount Number of objects to preallocate (variables/commands)
         * @param poolSize Number of chunks in the pool to preallocate (for hash map)
         */
        explicit ConsoleManager(IAllocator& allocator,
                                uint32 objectsCount = INITIAL_OBJECTS_COUNT,
                                uint32 poolSize = POOL_CHUNKS_COUNT,
                                uint32 historySize = HISTORY_PREALLOCATE);

        ~ConsoleManager() override;

        /** @copydoc IConsoleManager::registerVariable() */
        IConsoleVariable* registerVariable(const char *name, int32 initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerVariable() */
        IConsoleVariable* registerVariable(const char *name, float32 initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerVariable() */
        IConsoleVariable* registerVariable(const char *name, const char *initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerCommand() */
        IConsoleCommand* registerCommand(const char *name, const char *help,
                                         IConsoleCommand::ExecuteFunction function,
                                         EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::findObject() */
        IConsoleObject* findObject(const char *name) override;

        /** @copydoc IConsoleManager::findVariable() */
        IConsoleVariable* findVariable(const char *name) override;

        /** @copydoc IConsoleManager::findCommand() */
        IConsoleCommand* findCommand(const char *name) override;

        /** @copydoc IConsoleManager::processInput() */
        bool processInput(const char *input, IOutputDevice &device) override;

        /** @copydoc IConsoleManager::getConsoleHistory() */
        void getConsoleHistory(TArray<String> &out) override;

        /** @copydoc IConsoleManager::clearConsoleHistory() */
        void clearConsoleHistory() override;

    private:

        /** Implementation of find object without mutex */
        IConsoleObject* findObjectInternal(const String &name);

        /** String priority text fot output */
        const char* priorityToString(EConsolePriority priority);

    private:

        /** To pre-allocate memory for variables and objects */
        static const uint32 INITIAL_OBJECTS_COUNT = 16; /* 128; */

        /** To pre-allocate memory in the internal pool */
        static const uint32 POOL_CHUNKS_COUNT = 16; /* 128; */

        /** How much history entry to preallocate */
        static const uint32 HISTORY_PREALLOCATE = 16; /* 128; */

        /** Access/operations */
        Mutex mMutex;

        /** For memory operations */
        IAllocator& mAllocator;

        /** For objects container memory allocations */
        PoolAllocator mMemoryPool;

        /** Successfully executed string commands */
        TArray<String> mHistory;

        /** Store all variables and commands */
        THashMap<String,IConsoleObject*> mObjects;

    };

} // namespace Berserk

#endif //BERSERK_CONSOLEMANAGER_H