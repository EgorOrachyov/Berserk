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

    bool ClassManager::hasClass(const Berserk::CString &className) const {
        Guard guard(mAccessMutex);
        return mClasses.contains(className);
    }

    void ClassManager::registerClass(Berserk::TPtrShared<Berserk::Class> classInstance) {
        Guard guard(mAccessMutex);

        BERSERK_COND_ERROR_RET(classInstance.isNotNull(), "Passed null class instance");
        BERSERK_COND_ERROR_RET(!mClasses.contains(classInstance->getClassName()), "Class %s already registered", classInstance->getClassName().data());

        mClasses.add(classInstance->getClassName(), classInstance);
    }

    Class& ClassManager::getClass(const Berserk::CString &className) {
        Guard guard(mAccessMutex);

        BERSERK_COND_ERROR_FAIL(mClasses.contains(className), "No such class %s", className.data());
        return *mClasses[className];
    }

    TRef<Class> ClassManager::getClassPtr(const Berserk::CString &className) {
        Guard guard(mAccessMutex);

        auto ref = mClasses.getPtr(className);
        return ref.isNotNull() ? ref->getPtr(): nullptr;
    }

    ClassManager& ClassManager::getSingleton() {
        return *gClassManager;
    }

}