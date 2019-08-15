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
        virtual const String &getType() const = 0;

        /**
         * Returns type name of this object [must be unique in used namespace]
         * @return Type name of the object [raw c-style  string for debug]
         */
        const char* getType_str() const { return getType().get(); }

        /**
         * Return unique type id of the object
         * @return Object id
         */
        virtual uint32 getTypeId() const  = 0;

    };


    /** Assign class types id in run-time */
    class ReflectableIDs final
    {
    public:

        /** @return Current type id and increments global counter */
        static uint32 getNextTypeId();

    private:

        /** Id counter for all reflectable objects of that type */
        static uint32 TYPE_ID_COUNTER;

    };


    /**
     * Use this macro in h file to override class type info
     */
    #define REFLECTABLE_OBJECT(OBJECT_CLASS) \
    public: \
            const String& getType() const override { return OBJECT_CLASS ## _TYPENAME ; } \
            uint32 getTypeId() const override { return OBJECT_CLASS ## _TYPEID ; }; \
    private: \
            static const String OBJECT_CLASS ## _TYPENAME; \
            static const uint32 OBJECT_CLASS ## _TYPEID;

    /**
     * Use this macro in cpp file to initialize static members of the class for reflection
     */
    #define REFLECTABLE_OBJECT_INIT(OBJECT_CLASS) \
            const String OBJECT_CLASS:: OBJECT_CLASS ## _TYPENAME = #OBJECT_CLASS; \
            const uint32 OBJECT_CLASS:: OBJECT_CLASS ## _TYPEID = ReflectableIDs::getNextTypeId();


} // namespace Berserk

#endif //BERSERK_REFLECTABLE_H
