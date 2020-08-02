/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/Class.h>
#include <Reflection/ClassManager.h>

namespace Berserk {

    void Class::getInheritedClasses(TArray<Class *> &classes) const {
        auto current = getSuperClassPtr();
        while (current != nullptr) {
            classes.add(current.getPtr());
            current = current->getSuperClassPtr();
        }
    }

    bool Class::isSubclassOf(Class &predecessor) const {

        auto current = getSuperClassPtr();
        while (current != nullptr) {
            if (current == &predecessor)
                return true;

            current = current->getSuperClassPtr();
        }

        return false;
    }

    bool Class::hasMethod(const Berserk::CString &methodName) const {
        return mMethods.contains(methodName);
    }

    bool Class::hasProperty(const Berserk::CString &propertyName) const {
        return mProperties.contains(propertyName);
    }

    bool Class::hasPropertyGetSet(const Berserk::CString &propertyName) const {
        return mPropertiesGetSets.contains(propertyName);
    }

    const Method& Class::getMethod(const Berserk::CString &methodName) const {
        BERSERK_COND_ERROR_FAIL(hasMethod(methodName), "No such method %s::%s", getClassName().data(), methodName.data());
        return *mMethods.getPtr(methodName);
    }

    Class& Class::getSuperClass() const {
        return ClassManager::getSingleton().getClass(getSuperClassName());
    }

    TRef<Class> Class::getSuperClassPtr() const {
        return ClassManager::getSingleton().getClassPtr(getSuperClassName());
    }

    void Class::showDebugInfo() const {
        printf("Class: %s\n", getClassName().data());
        printf(" SuperClass: %s\n", (hasSuperClass()? getSuperClassName().data(): "(None)"));
        printf(" UUID: %s\n", getClassUUID().toString().data());

        TArray<Class*> inherits;
        getInheritedClasses(inherits);

        printf(" Inherits: (%u)\n", inherits.size());
        for (auto c: inherits) {
            printf("  %s\n", c->getClassName().data());
        }

        printf(" Properties: (%u)\n", mProperties.size());
        for (auto& e: mProperties) {
            e.second().showDebugInfo();
        }

        printf(" Methods: (%u)\n", mMethods.size());
        for (auto& e: mMethods) {
            e.second().showDebugInfo();
        }
    }

}