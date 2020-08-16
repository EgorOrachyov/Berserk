/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/ClassManager.h>

namespace Berserk {

    ClassManager* ClassManager::gClassManager = nullptr;

    ClassManager::ClassManager() {
        if (gClassManager) {
            fprintf(stderr, "[BERSERK Core] Allowed only single ClassManager instance");
            return;
        }

        gClassManager = this;
    }

    ClassManager::~ClassManager() {
        if (gClassManager == this)
            gClassManager = nullptr;
    }

    bool ClassManager::hasClass(const CString &className) const {
        Guard guard(mAccessMutex);
        return mClasses.contains(className);
    }

    void ClassManager::registerClass(TPtrShared<Class> classInstance) {
        Guard guard(mAccessMutex);

        BERSERK_COND_ERROR_RET(classInstance.isNotNull(), "Passed null class instance");
        BERSERK_COND_ERROR_RET(!mClasses.contains(classInstance->getClassName()), "Class %s already registered", classInstance->getClassName().data());

        mClasses.add(classInstance->getClassName(), classInstance);
    }

    void ClassManager::getRegisteredClasses(TArray<Class*> classes) {
        Guard guard(mAccessMutex);

        classes.ensureToAdd(mClasses.size());
        for (auto& entry: mClasses) {
            classes.add(entry.second().getPtr());
        }
    }

    Class& ClassManager::getClass(const CString &className) {
        Guard guard(mAccessMutex);

        BERSERK_COND_ERROR_FAIL(mClasses.contains(className), "No such class %s", className.data());
        return *mClasses[className];
    }

    TRef<Class> ClassManager::getClassPtr(const CString &className) {
        Guard guard(mAccessMutex);

        auto ref = mClasses.getPtr(className);
        return ref.isNotNull() ? ref->getPtr(): nullptr;
    }

    void ClassManager::showDebugInfo() {
        TArray<Class*> classesToShow;
        getRegisteredClasses(classesToShow);

        for (auto c: classesToShow) {
            c->showDebugInfo();
        }
    }

    ClassManager& ClassManager::getSingleton() {
        return *gClassManager;
    }

}