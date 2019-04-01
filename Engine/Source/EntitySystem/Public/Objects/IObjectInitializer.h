//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_IOBJECTINITIALIZER_H
#define BERSERK_IOBJECTINITIALIZER_H

#include <Misc/Types.h>
#include <Memory/IAllocator.h>
#include <Memory/PoolAllocator.h>
#include <Memory/StackAllocator.h>
#include <Memory/LinearAllocator.h>
#include <Base/XMLNode.h>
#include <Strings/String.h>

namespace Berserk::EntitySystem
{

    /**
     * An abstract object initializer, which contains common data, needed
     * for proper initialization of entities, components, systems, etc.
     */
    class ENGINE_API IObjectInitializer
    {
    public:

        /** Default object initializer from string name */
        explicit IObjectInitializer(const char* name);

        virtual ~IObjectInitializer() = default;

        /** @return Initialization scene node for object */
        virtual XMLNode* getXMLNode()                   { return mObjectNode; }

        /** @return General purpose allocator for objects tasks */
        virtual IAllocator* getAllocator()              { return mGenPurposeAllocator; }

        /** @return Pool Allocator [or nullptr] */
        virtual PoolAllocator* getPoolAllocator()       { return mObjectPool; }

        /** @return Stack Allocator [or nullptr] */
        virtual StackAllocator* getStackAllocator()     { return mObjectStack; }

        /** @return Linear Allocator [or nullptr] */
        virtual LinearAllocator* getLinearAllocator()   { return mObjectLinearAllocator; }

        /** @return Object name */
        virtual const char* getName()                   { return mObjectName.get(); }

    protected:

        CName mObjectName;                                  //! Limited string name of the object
        XMLNode* mObjectNode                    = nullptr;  //! XML node, which could be used by object to be initialized
        IAllocator* mGenPurposeAllocator        = nullptr;  //! General purpose allocator for object [if needed]
        PoolAllocator* mObjectPool              = nullptr;  //! Pool allocator for object tasks [if needed]
        StackAllocator* mObjectStack            = nullptr;  //! Stack allocator for object tasks [if needed]
        LinearAllocator* mObjectLinearAllocator = nullptr;  //! Linear allocator for object tasks [if needed]

    };

} // namespace Berserk::EntitySystem




#endif //BERSERK_IOBJECTINITIALIZER_H
