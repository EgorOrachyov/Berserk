/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <CoreRegisterClasses.h>
#include <Reflection/Object.h>
#include <Resource.h>

namespace Berserk {

    void CoreRegisterClasses::registerClasses() {
        Object::registerInfo();
        Resource::registerInfo();
    }

}