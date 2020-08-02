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

        /** @return */
        const Method& getSetter() const { return *mSet; }

        /** @return */
        const Method& getGetter() const { return *mGet; }

    private:
        template <typename T>
        friend class ClassBuilder;

        Method* mSet;
        Method* mGet;
    };

}

#endif //BERSERK_PROPERTYGETSET_H