//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_CONSOLEMANAGER_H
#define BERSERK_CONSOLEMANAGER_H

#include <Console/IConsoleManager.h>
#include <Containers/THashMap.h>
#include <Logging/Debug.h>

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
                                ILogManager& log = Debug::get(),
                                uint32 objectsCount = INITIAL_OBJECTS_COUNT,
                                uint32 poolSize = POOL_CHUNKS_COUNT,
                                uint32 historySize = HISTORY_PREALLOCATE);

        ~ConsoleManager() override;

        /** @copydoc IConsoleManager::registerVariable() */
        const IConsoleVariable* registerVariable(const char *name, int32 initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerVariable() */
        const IConsoleVariable* registerVariable(const char *name, float32 initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerVariable() */
        const IConsoleVariable* registerVariable(const char *name, const char *initialValue, const char *help,
                                           IConsoleVariable::OnChangeCallback callback,
                                           EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::registerCommand() */
        const IConsoleCommand* registerCommand(const char *name, const char *help,
                                         IConsoleCommand::ExecuteFunction function,
                                         EConsoleObjectFlags flags, EConsolePriority priority) override;

        /** @copydoc IConsoleManager::findObject() */
        const IConsoleObject* findObject(const char *name) const override;

        /** @copydoc IConsoleManager::findVariable() */
        const IConsoleVariable* findVariable(const char *name) const override;

        /** @copydoc IConsoleManager::findCommand() */
        const IConsoleCommand* findCommand(const char *name) const override;

        /** @copydoc IConsoleManager::processInput() */
        bool processInput(const char *input, IOutputDevice &device) override;

        /** @copydoc IConsoleManager::getConsoleHistory() */
        void getConsoleHistory(TArray<String> &out) const override;

        /** @copydoc IConsoleManager::clearConsoleHistory() */
        void clearConsoleHistory() override;

    private:

        /** Implementation of find object without mutex */
        IConsoleObject* findObjectInternal(const String &name) const;

        /** String priority text fot output */
        const char* priorityToString(EConsolePriority priority) const;

    private:

        /** To pre-allocate memory for variables and objects */
        static const uint32 INITIAL_OBJECTS_COUNT = 16; /* 128; */

        /** To pre-allocate memory in the internal pool */
        static const uint32 POOL_CHUNKS_COUNT = 16; /* 128; */

        /** How much history entry to preallocate */
        static const uint32 HISTORY_PREALLOCATE = 16; /* 128; */

        /** Access/operations */
        mutable Mutex mMutex;

        /** Set/get vars mutex [bottleneck] */
        mutable Mutex mVarsMutex;

        /** Engine logger */
        ILogManager& mLogManager;

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