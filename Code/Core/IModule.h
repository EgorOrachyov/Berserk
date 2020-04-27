/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IMODULE_H
#define BERSERK_IMODULE_H

#include <Typedefs.h>

namespace Berserk {

    /**
     * @brief Generic module interface
     *
     * Module interface/base class, which allows to auto registers
     * and track all the engine modules.
     */
    class IModule {
    public:

        IModule();

        virtual ~IModule();

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

        /** @return True if this module requires pre-update */
        virtual bool requiresPreUpdate() const = 0;

        /** @return True if this module requires post-update */
        virtual bool requiresPostUpdate() const = 0;

        /** @return Module name for look-up */
        virtual const char* getModuleName() const = 0;

        /** @return Module optional text description */
        virtual const char* getModuleDescription() const = 0;

        /** @return Module by name (null if not found) */
        static IModule* getModule(const char* name);

    private:

        /** Registers module in the global module entry list */
        void registerModule();

        /** Unregisters module (always called on unload) */
        void unregisterModule();

    };

}

#endif //BERSERK_IMODULE_H