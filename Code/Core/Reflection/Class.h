/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CLASS_H
#define BERSERK_CLASS_H

#include <TRef.h>
#include <UUID.h>
#include <Reflection/Method.h>
#include <Reflection/PropertyGetSet.h>

namespace Berserk {

    /**
     * @brief Reflection structure
     *
     * Describes engine game objects and resources structure
     * for serialization and UI visualization.
     *
     * Allows to query in run-time inheritance structure,
     * properties of the objects and call its public methods.
     */
    class Class final {
    public:

        /**
         * Collect inherited classes by this class
         * @param classes Array to store inherited class (Empty if this class is super class)
         */
        void getInheritedClasses(TArray<Class *> &classes) const;

        /**
         * Checks whether this class has predecessor in its inheritance three.
         * @param predecessor Class to check if this class inherits
         * @note This class isn not subclass of this class
         * @return True if this class is subclass of predecessor
         */
        bool isSubclassOf(Class& predecessor) const;

        /** @return True if has method of specified name (case-sensitive) */
        bool hasMethod(const CString& methodName) const;

        /** @return True if */
        bool hasProperty(const CString& propertyName) const;

        /** @return True if */
        bool hasPropertyGetSet(const CString& propertyName) const;

        /** @return True if has super class */
        bool hasSuperClass() const { return mHasSuperClass; }

        /** @return Object instance size in bytes */
        uint32 getObjectSize() const { return mObjectSize; }

        /** @return Property reference by name */
        const Property& getProperty(const CString& propertyName) const;

        /** @return Property get/set reference by name */
        const PropertyGetSet& getPropertyGetSet(const CString& propertyName) const;

        /** @return Method reference by name */
        const Method& getMethod(const CString& methodName) const;

        /** @return Externally created class UUID (stays the same among app calls) */
        const UUID &getClassUUID() const { return mID; }

        /** @return Class name */
        const CString &getClassName() const { return mClassName; }

        /** @return Super class name for this class (empty for root class) */
        const CString& getSuperClassName() const { return mSuperClassName; }

        /** @return Super class reference */
        Class& getSuperClass() const;

        /** @return Super class reference (ptr might be null for root) */
        TRef<Class> getSuperClassPtr() const;

        /** Debug print all class info */
        void showDebugInfo() const;

    private:
        template <typename T>
        friend class ClassBuilder;

        UUID mID;
        CString mClassName;
        CString mSuperClassName;
        bool mHasSuperClass = false;
        uint32 mObjectSize = 0;
        TMap<CString,Method> mMethods;
        TMap<CString,Property> mProperties;
        TMap<CString,PropertyGetSet> mPropertiesGetSets;
    };

}

#endif //BERSERK_CLASS_H