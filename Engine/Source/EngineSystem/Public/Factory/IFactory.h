//
// Created by Egor Orachyov on 02.04.2019.
//

#ifndef BERSERK_IFACTORY_H
#define BERSERK_IFACTORY_H

#include <Foundation/IObject.h>
#include <Factory/IFactoryInitializer.h>

namespace Berserk::EngineSystem
{

    /**
     * Abstract factory interface, which allows to define, how an
     * object with defined type name could be build in runtime mode.
     *
     * @note Used primary by SceneManager
     */
    class IFactory
    {
    public:

        /**
         * Allows to object of concrete type from initializer info.
         * Should be used primary for creating instances of IEntity and
         * IEntityComponent super classes.
         *
         * @param initializer Info needed to create object
         * @return Allocated and initialized by initializer object of type T
         */
        virtual IObject* CreateObject(const IFactoryInitializer& initializer) = 0;

        /** @return String representation type of objects, which created by this factory  */
        virtual const char* getTypeString() = 0;

        /** @return True, if type == its factory's objects type */
        virtual bool checkFromString(const char* type) = 0;

    };

} // namespace Berserk::EntitySystem

#endif //BERSERK_IFACTORY_H