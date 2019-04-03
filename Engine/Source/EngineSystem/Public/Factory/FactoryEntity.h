//
// Created by Egor Orachyov on 03.04.2019.
//

#ifndef BERSERK_FACTORYENTITY_H
#define BERSERK_FACTORYENTITY_H

#include <Factory/IFactory.h>
#include <Foundation/Entity.h>

namespace Berserk::EntitySystem
{

    /** Factory for IEntity object */
    class FactoryEntity : public IFactory
    {
    public:

        GENERATE_FACTORY_BODY(Entity);

        /** @copydoc IFactory::CreateObject() */
        IObject* CreateObject(const IFactoryInitializer& initializer) override;

        /** @return Instance of this factory  */
        static IFactory* getSingleton();

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_FACTORYENTITY_H
