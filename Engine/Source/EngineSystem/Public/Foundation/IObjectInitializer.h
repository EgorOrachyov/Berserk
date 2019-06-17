//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_IOBJECTINITIALIZER_H
#define BERSERK_IOBJECTINITIALIZER_H

#include <Misc/Types.h>
#include <XMLNode.h>
#include <Strings/String.h>
#include <Foundation/Forward.h>
#include <Memory/IAllocator.h>
#include <Memory/Old/PoolAllocator.h>
#include <Memory/Old/StackAllocator.h>
#include <Memory/Old/LinearAllocator.h>

namespace Berserk::Engine
{

    /**
     * An abstract object initializer, which contains common data, needed
     * for proper initialization of entities, components, systems, etc.
     *
     * @note Allowed only string names less than 64 chars of the length
     */
    class ENGINE_API IObjectInitializer
    {
    public:

        /** Default object initializer from string name */
        explicit IObjectInitializer(const char *name, IAllocator *general);

        virtual ~IObjectInitializer() = default;

        /** @return Pointer to object, which initializes this one */
        virtual IObject* getRoot() const                     { return mRootInitializer; }

        /** @return Initialization scene node for object */
        virtual XMLNode* getXMLNode() const                  { return mObjectNode; }

        /** @return General purpose allocator for objects tasks */
        virtual IAllocator* getAllocator() const             { return mGenPurposeAllocator; }

        /** @return Pool Allocator [or nullptr] */
        virtual PoolAllocator* getPoolAllocator() const      { return mObjectPool; }

        /** @return Stack Allocator [or nullptr] */
        virtual StackAllocator* getStackAllocator() const    { return mObjectStack; }

        /** @return Linear Allocator [or nullptr] */
        virtual LinearAllocator* getLinearAllocator() const  { return mObjectLinearAllocator; }

        /** @return Object name */
        virtual const char* getName() const                  { return mObjectName.get(); }

    private:

        CName mObjectName;                                  //! Limited string name of the object
        IObject* mRootInitializer               = nullptr;  //! Who initializes
        XMLNode* mObjectNode                    = nullptr;  //! XML node, which could be used by object to be initialized
        IAllocator* mGenPurposeAllocator        = nullptr;  //! General purpose allocator for object [if needed]
        PoolAllocator* mObjectPool              = nullptr;  //! Pool allocator for object tasks [if needed]
        StackAllocator* mObjectStack            = nullptr;  //! Stack allocator for object tasks [if needed]
        LinearAllocator* mObjectLinearAllocator = nullptr;  //! Linear allocator for object tasks [if needed]

    };

} // namespace Berserk::EntitySystem




#endif //BERSERK_IOBJECTINITIALIZER_H
