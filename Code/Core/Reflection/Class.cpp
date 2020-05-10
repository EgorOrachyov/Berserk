/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <Reflection/Class.h>

namespace Berserk {

    void Class::inheritedClasses(TArray<Class *> &classes) {
        Class* current = mSuperclass;
        while (current != nullptr) {
            classes.add(current);
            current = current->mSuperclass;
        }
    }

    bool Class::isSubclassOf(Class &predecessor) {
        Class* current = mSuperclass;
        while (current != nullptr) {
            if (current == &predecessor)
                return true;

            current = current->mSuperclass;
        }

        return false;
    }

    bool Class::hasMethod(const char *method) const {
        for (const auto& m: mClassMethods) {
            if (m.getMethodName() == method)
                return true;
        }

        return false;
    }

    bool Class::hasField(const char *field) const {
        for (const auto& f: mClassFields) {
            if (f.getFieldName() == field)
                return true;
        }

        return false;
    }

}