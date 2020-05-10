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

namespace Berserk {

    /** @brief Generic object class */
    class Object {
    public:
        virtual ~Object() = default;

        /**
         * Serialize an object to arbitrary archive
         * @param archive Archive to write data
         */
        virtual void serialize(Archive& archive) = 0;

        /**
         * Access class reflection data of this object
         * @note Classes must be dynamically allocated instead of static creation to support modules reload.
         * @return Class instance of this object
         */
        virtual class Class& getClass() = 0;
    };

}

#endif //BERSERK_OBJECT_H
