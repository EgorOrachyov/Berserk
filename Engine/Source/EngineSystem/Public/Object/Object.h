//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_OBJECT_H
#define BERSERK_OBJECT_H

#include <Object/Allocatable.h>
#include <Object/Reflectable.h>
#include <Object/ObjectID.h>
#include <Engine/IEnvironment.h>
#include "ObjectInitializer.h"

namespace Berserk
{

    /**
     * The base class for all engine game-play foundation system objects
     * and components. Root class for all objects in the entity component engine system.
     *
     * @note Provides unified access to all engine run-time systems and
     *       services. Must be multi-threaded and support 'mark dirty' modification
     *       model with the following systems data synchronization.
     *
     * @note All objects of this supper class are handled by global objects
     *       entry. All the objects modifications will be handled by state manager (todo)
     */
    class ENGINE_API Object : public Allocatable, public IReflectable
    {
    public:

        REFLECTABLE_OBJECT(Object);

    public:

#if DEBUG
        Object() { }
#endif

        /** Construct object default data from initializer */
        explicit Object(const ObjectInitializer& initializer);

        /** All the subclasses ob object must declare virtual destructor */
        ~Object() override = default;

        /**
         * Called by object manager after object constructed, before the object is registered
         * in the main objects entry. Allows add control over object initialization.
         *
         * @note Internal API, called only by Object Manager
         */
        virtual void initialize() {};

        /**
         * Called by object manager after object unregistered from main object entry
         * and totally destruct object [it happens when level or level data is unloaded]
         *
         * @note Internal API, called only by Object Manager
         */
        virtual void destroy() {};

        /**
         * @return globally unique object ID. Each object could be successfully found
         *         in global objects entry via its id.
         *
         * @note Prefer store objects via its ObjectID instead of shared/weak pointers because
         *       of object could be unloaded/destructed in time of the execution.
         */
        ObjectID getObjectID() const { return mObjectID; }

        /**
         * @return Object string name [could be not unique] for view/output debug purposes
         */
        const String& getObjectName() const { return mObjectName; }

        /**
         * @return Global engine environment - access to all the engine run-time systems
         */
        IEnvironment& getEnvironment() const { return *mEnvironment; }

        /**
         * @return Global engine object manager - entry for all the game objects
         */
        IObjectManager& getObjectManager() const { return mEnvironment->getObjectManager(); }

        /** @return This object dirty flags */
        uint32 getDirtyFlags() const { return mDirtyFlags; }

    protected:

        /** Mark this object specified flags dirty for future */
        void markDirty(uint32 flags);

        /** Marks this object flags clean */
        void markClean() { mDirtyFlags = 0; }

        /**
         * Object manager will trigger this method to sync this object data
         *
         * @note Because of multi-threaded execution model, each system, which operates
         *       object, stores its private copy. Therefore if object was changer or marked dirty
         *       there is need of data synchronization.
         *
         * @note Other systems could register their own listeners for objects
         *       modification. Listeners will be called after triggerSync method was executed.
         *
         * @note Override this method to sync components data
         *       with render or physics thread. Here your object can request external
         *       systems method to say, that its data was changed.
         *
         * @note All the objects trigger sync will be called in single thread
         *       mode after execution cycle pass, therefore there is no need in
         *       synchronization.
         */
        virtual void triggerSync() {};

    private:

        friend class ObjectManager;

        /** Dirty flags for this object */
        uint32 mDirtyFlags = 0;

        /** Object unique ID - generated in run-time */
        ObjectID mObjectID;

        /** Object name - assigned in construction/initialization time */
        String mObjectName;

        /** Global engine environment [injected in each game object] */
        IEnvironment* mEnvironment = nullptr;

    };

} // namespace Berserk

#endif //BERSERK_OBJECT_H