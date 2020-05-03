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
#include <TArray.h>
#include <Platform/Memory.h>
#include <Threading/TSync.h>

namespace Berserk {

    /**
     * @brief Generic module interface
     *
     * Module interface/base class, which allows to auto registers
     * and track all the engine modules.
     */
    class IModule {
    public:

        virtual ~IModule() = default;

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

        /** @return True, if module can be reloaded in run-time */
        virtual bool getSupportsReloading() const { return false; }

        /** @return Module name for look-up */
        virtual const char* getModuleName() const = 0;

        /** @return Name of the project, where this module was declared (modules could be shared among shared libs) */
        virtual const char* getModuleProjectName() const = 0;

        /** @return Module optional text description */
        virtual const char* getModuleDescription() const = 0;

    protected:

        friend class Engine;

        /** Registers module in the global module entry list */
        void registerModule();

        /** Unregisters module (always called on unload) */
        void unregisterModule();

    };

}

#endif //BERSERK_IMODULE_H