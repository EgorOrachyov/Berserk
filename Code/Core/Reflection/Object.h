/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_OBJECT_H
#define BERSERK_OBJECT_H

#include <IO/Archive.h>
#include <Reflection/ClassManager.h>

namespace Berserk {

/**
 * Use this macro to extend Object class and add new reflectable object into the engine class base.
 * This macro adds common class definitions, utility methods and inserts required names.
 */
#define BERSERK_CLASS(ClassName, SuperClassName)                                                                \
        public:                                                                                                 \
            class Class& getClass() const override { return getClassStatic(); }                                 \
            CString getClassName() const override { return #ClassName; }                                        \
            CString getSuperClassName() const override { return #SuperClassName; }                              \
            bool hasSuperClass() const override { return true; }                                                \
            static class Class& getClassStatic() { return ClassManager::getSingleton().getClass(#ClassName); }  \
            static CString getClassNameStatic() { return #ClassName; }                                          \
            static CString getSuperClassNameStatic() { return #SuperClassName; }                                \
            static bool hasSuperClassStatic() { return true; }


    /** @brief Generic object class */
    class Object {
    public:
        virtual ~Object() = default;

        /** @return Class info instance about this object */
        virtual class Class& getClass() const;

        /** @return Class name string of this object */
        virtual CString getClassName() const { return "Object"; }

        /** @return Superclass name if present */
        virtual CString getSuperClassName() const { return ""; }

        /** @return True if ha super class */
        virtual bool hasSuperClass() const { return false; }

        /** @return Class info instance about this object (static) */
        static class Class& getClassStatic();

        /** @return Class name string of this object type (static) */
        static CString getClassNameStatic() { return "Object"; }

        /** @return Superclass name if present */
        static CString getSuperClassNameStatic() { return ""; }

        /** @return True if ha super class */
        static bool hasSuperClassStatic() { return false; }

        /** Called once to register this object class */
        static void registerInfo();

        /** Call specified object method */
        EError callMethod(const CString &methodName, const TArrayStatic<Variant *, MethodInfo::MAX_ARGS> &args, Variant &ret);

        /** Get property value by name */
        EError getProperty(const CString &propertyName, Variant& ret);

        /** Set property value by name */
        EError setProperty(const CString &propertyName, Variant& value);

    private:

        template <typename T>
        friend class ClassBuilder;

    };

}

#endif //BERSERK_OBJECT_H
