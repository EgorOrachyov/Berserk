/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CLASSMANAGER_H
#define BERSERK_CLASSMANAGER_H

#include <Reflection/Class.h>
#include <TPtrShared.h>

namespace Berserk {

    /**
     * @brief Class manager
     *
     * Globally accessible engine class manager, which stores at runtime
     * all the class info. Class info provided for objects, which are subclass on the Object base type.
     */
    class ClassManager {
    public:

        ClassManager();
        ~ClassManager();

        /** @return True if class registered in the manager */
        bool hasClass(const CString &className) const;

        /** Register new class in the manager */
        void registerClass(TPtrShared<Class> classInstance);

        /** Dumps registered in the manager classes */
        void getRegisteredClasses(TArray<Class*> classes);

        /** @return Class instance reference */
        Class& getClass(const CString& className);

        /** @return Class instance reference (ptr might be null) */
        TRef<Class> getClassPtr(const CString& className);

        /** Show info about all classes */
        void showDebugInfo();

        /** @return Global class manager instance */
        static ClassManager& getSingleton();

    private:

        mutable Mutex mAccessMutex;
        TMap<CString,TPtrShared<Class>> mClasses;

        /** Global instance */
        static ClassManager* gClassManager;

    };


}

#endif //BERSERK_CLASSMANAGER_H