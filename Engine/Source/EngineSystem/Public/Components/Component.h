//
// Created by Egor Orachyov on 2019-07-31.
//

#ifndef BERSERK_COMPONENT_H
#define BERSERK_COMPONENT_H

#include <Object/Object.h>

namespace Berserk
{

    /**
     * Base class for any Entity object. Only instances of classes derived from
     * Component could be attached to the entity. Components describes any
     * common data and processing params, which could be shared among entities.
     *
     * @note Components do not have any game logic. They store only data, which
     *       could be accessed via entity and its behaviour classes/logic/scripts.
     */
    class ENGINE_API Component : public Object
    {
    public:

        REFLECTABLE_OBJECT(Component);

    public:

#if DEBUG
        Component() : Object() {}
#endif

        ~Component() override = default;

        // todo: add common functionality an methods
        // todo: transform update
        // todo: entity handle ref

    };

} // namespace Berserk

#endif //BERSERK_COMPONENT_H