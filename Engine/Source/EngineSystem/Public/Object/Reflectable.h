//
// Created by Egor Orachyov on 2019-08-02.
//

#ifndef BERSERK_REFLECTABLE_H
#define BERSERK_REFLECTABLE_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>
#include <Strings/String.h>
#include <typeinfo>

namespace Berserk
{

    /**
     * Base object containing run-time class info. Allows to sort object via theirs types.
     * @note Functionality will be added later
     */
    class ENGINE_API IReflectable
    {
    public:

        virtual ~IReflectable() = default;

        /**
         * Returns type name of this object [must be unique in used namespace]
         * @return Type name of the object
         */
        virtual String getType() const = 0;

        /**
         * Return unique type id of the object
         * @return Object id
         */
        virtual uint32 getTypeId() const  = 0;

        /** @return Current type id and increments global counter */
        static uint32 getNextTypeId() { return TYPE_ID_COUNTER++; }

    private:

        /** Id counter for all reflectable objects of that type */
        static uint32 TYPE_ID_COUNTER;

    };

    uint32 IReflectable::TYPE_ID_COUNTER = 0;

    template <class T>
    class ENGINE_API Reflectable : public IReflectable
    {
    public:

        virtual ~Reflectable() = default;

        /** @copydoc IReflectable::getType() */
        String getType() const override { return String(typeid(T).name()); }

        /** @copydoc IReflectable::getTypeId() */
        uint32 getTypeId() const override { return mTypeId; }

        /** @return Static type id */
        static uint32 getClass_TypeId() { return mTypeId; }

    private:

        static const uint32 mTypeId;

    };

    template <class T>
    const uint32 Reflectable<T>::mTypeId = IReflectable::getNextTypeId();

} // namespace Berserk

#endif //BERSERK_REFLECTABLE_H
