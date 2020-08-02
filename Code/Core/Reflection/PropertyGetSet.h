/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PROPERTYGETSET_H
#define BERSERK_PROPERTYGETSET_H

#include <Reflection/Property.h>
#include <Reflection/Method.h>

namespace Berserk {

    class PropertyGetSet {
    public:

        /** @return True if setter function provided */
        bool isSetterProvided() const { return mSet != nullptr; }

        /** @return True if getter function provided */
        bool isGetterProvided() const { return mSet != nullptr; }

        /** @return Setter method */
        const Method& getSetter() const { return *mSet; }

        /** @return Getter method */
        const Method& getGetter() const { return *mGet; }

        /** @return Property which is accessed by this get/set methods */
        const Property& getProperty() const { return *mProperty; }

        /** Print debug info */
        void showDebugInfo() const {
            printf("  Property: %s, get: %s, set: %s\n",
                    mProperty->getName().data(),
                    isGetterProvided()? mGet->getName().data(): "(None)",
                    isSetterProvided()? mSet->getName().data(): "(None)");
        }

    private:
        template <typename T>
        friend class ClassBuilder;

        const Property* mProperty = nullptr;
        const Method* mSet = nullptr;
        const Method* mGet = nullptr;
    };

}

#endif //BERSERK_PROPERTYGETSET_H