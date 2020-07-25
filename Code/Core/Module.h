/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MODULE_H
#define BERSERK_MODULE_H

#include <TRef.h>
#include <Containers/TArray.h>
#include <Platform/Memory.h>
#include <Platform/TSync.h>

namespace Berserk {

    /**
     * @brief Generic module interface
     *
     * Module interface/base class, which allows to auto registers
     * and track all the engine modules.
     */
    class Module {
    public:

        virtual ~Module() = default;

        /**
         * @brief Module post-load event
         *
         * Called once when module is loaded to the engine.
         */
        virtual void onPostLoad() = 0;

        /**
         * @brief Module pre-unload event
         *
         * Called once when module is going to be unloaded.
         * After this module will be closed and unloaded.
         */
        virtual void onPreUnload() = 0;

        /** @return True if module could be unloaded at runtime */
        virtual bool canUnload() const { return false; }

        /** @return True, if module can be reloaded in run-time */
        virtual bool getSupportsReloading() const { return false; }

        /** @return Module name for look-up */
        virtual const char* getModuleName() const = 0;

        /** @return Name of the project, where this module was declared (modules could be shared among shared libs) */
        virtual const char* getModuleProjectName() const = 0;

        /** @return Module optional text description */
        virtual const char* getModuleDescription() const = 0;

    };

}

#endif //BERSERK_MODULE_H