/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IMODULE_H
#define BERSERK_IMODULE_H

#include <TRef.h>
#include <Typedefs.h>
#include <Platform/Memory.h>

namespace Berserk {

    /**
     * @brief Generic module interface
     *
     * Module interface/base class, which allows to auto registers
     * and track all the engine modules.
     */
    class IModule {
    protected:

        IModule();

        virtual ~IModule();

        /**
         * @brief Engine start-up event
         *
         * Called once when engine has initialized all the systems and modules.
         * No game logic processed before this step.
         * After initialize step engine enters main loop update process.
         *
         * @note In this step all the engine default modules are available
         */
        virtual void onPostInitialize() = 0;

        /**
         * @brief Engine shut-down event
         *
         * Called once when engine is closing.
         * No game logic processed late.
         * After finalize step all the engine modules will be closed and unloaded.
         *
         * @note In this step all the engine default modules are available
         */
        virtual void onPostFinalize() = 0;

        /**
         * @brief Engine pre-update event handler
         *
         * Called for each module if required in the pre-update engine phase.
         * Pre-update event happens in the beginning of the frame process before scenes traversal and update.
         *
         * Override this method in order to run custom logic here.
         *
         * @note Keep this method execution as fast as possible since this is called on each module in the single thread.
         * @note For computationally hard tasks prefer submitting jobs in the job manager.
         */
        virtual void onPreUpdate() = 0;

        /**
         * @brief Engine post-update event handler
         *
         * Called for each module if required in the post-update engine phase.
         * Post-update event happens in the end of the frame process after all scenes are traversed and
         * scripting logic is executed.
         *
         * Override this method in order to run custom logic here.
         *
         * @note Keep this method execution as fast as possible since this is called on each module in the single thread.
         * @note For computationally hard tasks prefer submitting jobs in the job manager.
         */
        virtual void onPostUpdate() = 0;

    public:

        /** @return Module name for look-up */
        virtual const char* getModuleName() const = 0;

        /** @return Module optional text description */
        virtual const char* getModuleDescription() const = 0;

        /** @return Module by name (null if not found) */
        static TRef<IModule> getModule(const char* name);

    private:

        template <typename Module>
        friend class ModuleDeclareHelper;
        friend class Enigne;

        /** Registers module in the global module entry list */
        void registerModule();

        /** Unregisters module (always called on unload) */
        void unregisterModule();

    };

    /////////////////////////////////////////////////////////////////////////////////////////////

/** Declares and creates instance of the module */
#define BERSERK_DECLARE_MODULE(module)   \
    static ModuleDeclareHelper<module> gModuleInstance ## module;

/** Module instance reference */
#define BERSERK_MODULE_REFERENCE(module) \
    (gModuleInstance ## module).get()

    template <typename Module>
    class ModuleDeclareHelper {
    public:
        ModuleDeclareHelper() { module = new (memory) Module(); }
        ~ModuleDeclareHelper() { module->~IModule(); }
        Module& get() { return *((Module*) module); }
    private:
        uint8 memory[sizeof(Module)];
        IModule* module = nullptr;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////

}

#endif //BERSERK_IMODULE_H