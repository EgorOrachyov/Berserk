//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_ISYSTEM_H
#define BERSERK_ISYSTEM_H

#include <Foundation/Forward.h>
#include <Strings/String.h>
#include <Misc/UsageDescriptors.h>
#include <Foundation/IObjectMacros.h>
#include <Systems/ISystemInitializer.h>

namespace Berserk::EntitySystem
{

    /**
     * Base class for any kind of engine system class. Root class for rendering,
     * physics, audio, ai systems. Also super class for any entity and component
     * system worker.
     *
     * Defines, how to process, initialize, destroy, update objects in single and
     * multi-thread mode.
     */
    class ENGINE_API ISystem
    {
    public:

        /** Provide minimal required interface for memory operations */
        GENERATE_CLASS_BODY(ISystem);

        /** Do actually nothing */
        explicit ISystem(const ISystemInitializer& systemInitializer) { }

        /** Do actually nothing */
        virtual ~ISystem() = default;

    public:

        /** Called after system creation */
        virtual void initialize() = 0;

        /** Called [optionally] before all the update-pass in SINGLE-THREAD mode */
        virtual void preUpdate() = 0;

        /** Called each frame in main-update pass */
        virtual void update() = 0;

        /** Called [optionally] after all the update-pass in SINGLE-THREAD mode */
        virtual void postUpdate() = 0;

        /** Called before system delete */
        virtual void destroy() = 0;

    public:

        /** @return True if initialize was called */
        bool isInitialized() { return mIsInitialized; }

        /** @return True if destroy was called */
        bool isDestroyed() { return mIsDestroyed; }

        /** @return True if Can execute in Job System */
        bool isMultiThreaded() { return  mIsMultiThreaded; }

        /** @return True if needed preUpdate call before global update in frame */
        bool requiresPreUpdate() { return mRequiresPreUpdate; }

        /** @return True if needed postUpdate call after global update in frame */
        bool requiresPostUpdate() { return mRequiresPostUpdate; }

    protected:

        /** Was initialize called */
        uint8 mIsInitialized : 1;

        /** Was destroy called */
        uint8 mIsDestroyed : 1;

        /** Can execute in Job System */
        uint8 mIsMultiThreaded : 1;

        /** Need preUpdate call before global update in frame */
        uint8 mRequiresPreUpdate : 1;

        /** Need postUpdate call after global update in frame */
        uint8 mRequiresPostUpdate : 1;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_ISYSTEM_H